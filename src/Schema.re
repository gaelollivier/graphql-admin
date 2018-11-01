open Belt;

type field = {
  name: string,
  typeRef,
}
and inputField = {
  name: string,
  typeRef,
}
and type_ =
  | Scalar(string)
  | Object(string, list(field))
  | Interface(string)
  | Union(string)
  | Enum(string)
  | InputObject(string, list(inputField))
and typeRef =
  | Type(string)
  | List(typeRef)
  | NonNull(typeRef);

type t = {
  queryFields: list(field),
  types: Map.String.t(type_),
};

/* returns the name for a given type */
let getTypeName = (type_: type_) =>
  switch (type_) {
  | Scalar(name) => name
  | Object(name, _) => name
  | Interface(name) => name
  | Union(name) => name
  | Enum(name) => name
  | InputObject(name, _) => name
  };

/* Displayable types are scalars, enums, or list/non null of displayable type */
let rec isDisplayable = (schema: t, typeRef: typeRef) =>
  switch (typeRef) {
  | List(ref_)
  | NonNull(ref_) => isDisplayable(schema, ref_)
  | Type(name) =>
    let type_ = schema.types->Map.String.getExn(name);
    switch (type_) {
    | Scalar(_)
    | Enum(_) => true
    | Object(_, _)
    | Interface(_)
    | Union(_)
    | InputObject(_, _) => false
    };
  };

let unwrapNonNull = (typeRef: typeRef) =>
  switch (typeRef) {
  | NonNull(ref_) => ref_
  | _ => typeRef
  };

let introspectionQuery = "query IntrospectionQuery {
    __schema {
      queryType { name }
      mutationType { name }
      subscriptionType { name }
      types {
        ...FullType
      }
    }
  }

  fragment FullType on __Type {
    kind
    name
    description
    fields(includeDeprecated: true) {
      name
      description
      args {
        ...InputValue
      }
      type {
        ...TypeRef
      }
      isDeprecated
      deprecationReason
    }
    inputFields {
      ...InputValue
    }
    interfaces {
      ...TypeRef
    }
    enumValues(includeDeprecated: true) {
      name
      description
      isDeprecated
      deprecationReason
    }
    possibleTypes {
      ...TypeRef
    }
  }

  fragment InputValue on __InputValue {
    name
    description
    type { ...TypeRef }
    defaultValue
  }

  fragment TypeRef on __Type {
    kind
    name
    ofType {
      kind
      name
      ofType {
        kind
        name
        ofType {
          kind
          name
        }
      }
    }
}";

/* JSON decoding */

let rec decodeField = json =>
  Json.Decode.{
    name: json |> field("name", string),
    typeRef: json |> field("type", decodeTypeRef),
  }
and decodeInputField: Js.Json.t => inputField =
  json =>
    Json.Decode.{
      name: json |> field("name", string),
      typeRef: json |> field("type", decodeTypeRef),
    }
and decodeTypeRef = json => {
  open Json.Decode;
  let kind = json |> field("kind", string);
  switch (kind) {
  | "LIST" => List(json |> field("ofType", decodeTypeRef))
  | "NON_NULL" => NonNull(json |> field("ofType", decodeTypeRef))
  | _ => Type(json |> field("name", string))
  };
}
and decodeType = json => {
  open Json.Decode;
  let kind = json |> field("kind", string);
  switch (kind) {
  | "SCALAR" => Scalar(json |> field("name", string))
  | "OBJECT" =>
    Object(
      json |> field("name", string),
      json |> field("fields", list(decodeField)),
    )
  | "INTERFACE" => Interface(json |> field("name", string))
  | "UNION" => Union(json |> field("name", string))
  | "ENUM" => Enum(json |> field("name", string))
  | "INPUT_OBJECT" =>
    InputObject(
      json |> field("name", string),
      json |> field("fields", list(decodeInputField)),
    )
  | unknown =>
    raise(Json.Decode.DecodeError("Uknown type kind '" ++ unknown ++ "'"))
  };
};

let decodeIntrospectionQuery = (introspectionResult: Js.Json.t) => {
  /* decode schema types */
  let types =
    Json.Decode.(
      introspectionResult
      |> at(["data", "__schema", "types"], array(decodeType))
    );
  /* index them by name */
  let typesByName =
    types
    ->Array.map(type_ => (getTypeName(type_), type_))
    ->Map.String.fromArray;
  /* retrieve query type */
  let queryTypeName =
    Json.Decode.(
      introspectionResult
      |> at(["data", "__schema", "queryType", "name"], string)
    );
  let queryType = typesByName->Map.String.getExn(queryTypeName);
  switch (queryType) {
  | Object(_, fields) => {queryFields: fields, types: typesByName}
  | _ => raise(Json.Decode.DecodeError("Expected query type to be Object"))
  };
};

let rec decodeValue = (schema: t, typeRef: typeRef, json: Js.Json.t) =>
  Json.Decode.(
    switch (typeRef) {
    | NonNull(ref_) => decodeValue(schema, ref_, json)
    | List(ref_) =>
      (json |> list(x => x))
      ->List.map(value => decodeValue(schema, ref_, value))
      |> String.concat(", ")
    | Type("String") => string(json)
    | _ => Json.stringify(json)
    }
  );

let decodeField = (schema: t, typeRef: typeRef, name: string, json: Js.Json.t) => {
  let valueOpt =
    Json.Decode.(
      json |> optional(field(name, decodeValue(schema, typeRef)))
    );
  switch (valueOpt) {
  | None => "NULL"
  | Some(value) => value
  };
};