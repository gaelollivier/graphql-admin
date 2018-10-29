open Belt;

type typeKind =
  | Scalar
  | Object
  | Interface
  | Union
  | Enum
  | InputObject
  | List
  | NonNull;

type field = {
  name: string,
  type_,
}
and type_ = {
  name: option(string),
  kind: typeKind,
  fields: option(list(field)),
  ofType: option(type_),
};

type t = {
  queryFields: list(field),
  types: Map.String.t(type_),
};

let decodeKind = json =>
  switch (Json.Decode.string(json)) {
  | "SCALAR" => Scalar
  | "OBJECT" => Object
  | "INTERFACE" => Interface
  | "UNION" => Union
  | "ENUM" => Enum
  | "INPUT_OBJECT" => InputObject
  | "LIST" => List
  | "NON_NULL" => NonNull
  | unknown =>
    raise(Json.Decode.DecodeError("Uknown type kind '" ++ unknown ++ "'"))
  };

let rec decodeField = json =>
  Json.Decode.{
    name: json |> field("name", string),
    type_: json |> field("type", decodeType),
  }
and decodeType = json =>
  Json.Decode.{
    name: json |> optional(field("name", string)),
    kind: json |> field("kind", decodeKind),
    fields: json |> optional(field("fields", list(decodeField))),
    ofType: json |> optional(field("ofType", decodeType)),
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
    ->Array.map(type_ => (Option.getExn(type_.name), type_))
    ->Map.String.fromArray;
  /* retrieve query type */
  let queryTypeName =
    Json.Decode.(
      introspectionResult
      |> at(["data", "__schema", "queryType", "name"], string)
    );
  let queryType = typesByName->Map.String.getExn(queryTypeName);
  {queryFields: queryType.fields->Option.getExn, types: typesByName};
};

/* Displayable types are scalars, enums, or list/non null of displayable type */
let rec isDisplayable = (type_: type_) =>
  switch (type_) {
  | {kind: Scalar | Enum} => true
  | {kind: List | NonNull, ofType: Some(ofType)} => isDisplayable(ofType)
  | _ => false
  };

let rec decodeValue = (type_: type_, json: Js.Json.t) =>
  Json.Decode.(
    switch (type_) {
    | {kind: NonNull, ofType: Some(ofType)} => decodeValue(ofType, json)
    | {kind: List, ofType: Some(ofType)} =>
      (json |> list(x => x))->List.map(value => decodeValue(ofType, value))
      |> String.concat(", ")
    | {name: Some("String")} => string(json)
    | _ => Json.stringify(json)
    }
  );

let decodeField = (type_: type_, name: string, json: Js.Json.t) => {
  let valueOpt =
    Json.Decode.(json |> optional(field(name, decodeValue(type_))));
  switch (valueOpt) {
  | None => "NULL"
  | Some(value) => value
  };
};

let unwrapNonNull = (type_: type_) =>
  switch (type_) {
  | {kind: NonNull, ofType: Some(ofType)} => ofType
  | _ => type_
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