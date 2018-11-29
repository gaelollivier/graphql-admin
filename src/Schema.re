open Belt;

/* Represents a top-level graphql type, such as "String", "Account", "Query", ... */
type type_ =
  | Scalar(string)
  | Object(string, list(field))
  | Interface(string)
  | Union(string)
  | Enum(string)
  | InputObject(string, list(arg))
/* object field */
and field = {
  name: string,
  args: list(arg),
  typeRef,
}
/* argument (for fields or input objects) */
and arg = {
  name: string,
  typeRef,
}
/*
 * typeRef is used to reference field types. They are separated from main type
 * so we don't need to bother with List/NonNull for top-level types.
 * Also, referencing a top-level type is done through `Lazy` so we can fully
 * represent a schema with cyclic references
 */
and typeRef =
  | Type(Lazy.t(type_))
  | List(typeRef)
  | NonNull(typeRef);

/* schema type */
type t = {queryFields: list(field)};

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
let rec isDisplayable = (typeRef: typeRef) =>
  switch (typeRef) {
  | List(ref_)
  | NonNull(ref_) => isDisplayable(ref_)
  | Type(type_) =>
    let type_ = Lazy.force(type_);
    switch (type_) {
    | Scalar(_)
    | Enum(_) => true
    | Object(_, _)
    | Interface(_)
    | Union(_)
    | InputObject(_, _) => false
    };
  };

/* retrieve the underlying type of a type ref, unwrapping any list/non null specifiers */
let rec getReferencedTypeExn = (typeRef: typeRef) =>
  switch (typeRef) {
  | Type(ref_) => Lazy.force(ref_)
  | NonNull(ref_)
  | List(ref_) => getReferencedTypeExn(ref_)
  };

let getFieldsExn = (type_: type_) =>
  switch (type_) {
  | Object(_, fields) => fields
  | _ => raise(Invalid_argument("Expected object type"))
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

let rec decodeType = (getTypeExn, json) => {
  open Json.Decode;
  let kind = json |> field("kind", string);
  switch (kind) {
  | "SCALAR" => Scalar(json |> field("name", string))
  | "OBJECT" =>
    Object(
      json |> field("name", string),
      json |> field("fields", list(decodeField(getTypeExn))),
    )
  | "INTERFACE" => Interface(json |> field("name", string))
  | "UNION" => Union(json |> field("name", string))
  | "ENUM" => Enum(json |> field("name", string))
  | "INPUT_OBJECT" =>
    InputObject(
      json |> field("name", string),
      json |> field("inputFields", list(decodeArg(getTypeExn))),
    )
  | unknown =>
    raise(Json.Decode.DecodeError("Uknown type kind '" ++ unknown ++ "'"))
  };
}
and decodeField = (getTypeExn, json) =>
  Json.Decode.{
    name: json |> field("name", string),
    args: json |> field("args", list(decodeArg(getTypeExn))),
    typeRef: json |> field("type", decodeTypeRef(getTypeExn)),
  }
and decodeArg = (getTypeExn, json): arg =>
  Json.Decode.{
    name: json |> field("name", string),
    typeRef: json |> field("type", decodeTypeRef(getTypeExn)),
  }
and decodeTypeRef = (getTypeExn, json) => {
  open Json.Decode;
  let kind = json |> field("kind", string);
  switch (kind) {
  | "LIST" => List(json |> field("ofType", decodeTypeRef(getTypeExn)))
  | "NON_NULL" =>
    NonNull(json |> field("ofType", decodeTypeRef(getTypeExn)))
  | _ => Type(lazy (getTypeExn(json |> field("name", string))))
  };
};

let decodeIntrospectionQuery = (introspectionResult: Js.Json.t) => {
  /*
   * We will build map of all top-level types, indexed by name
   * The map is stored in a reference so we can capture-it within a closure
   * and pass-it along to JSON decoders. This way, type references within the
   * schema can be lazily resolved using the types map.
   */
  let typesByName = ref(Map.String.empty);
  let getTypeExn = name => (typesByName^)->Map.String.getExn(name);
  /* decode schema types */
  let types =
    Json.Decode.(
      introspectionResult
      |> at(["data", "__schema", "types"], array(decodeType(getTypeExn)))
    );
  /* index them by name */
  typesByName :=
    types
    ->Array.map(type_ => (getTypeName(type_), type_))
    ->Map.String.fromArray;
  /*
   * NOTE: as an additional validation check, we could traverse all types and
   * make sure that all type refs are valid (i.e: point to actual types in the types map)
   * Otherwise, we risk exceptions being thrown when lazily-resolving type references
   */
  let queryTypeName =
    Json.Decode.(
      introspectionResult
      |> at(["data", "__schema", "queryType", "name"], string)
    );
  switch (getTypeExn(queryTypeName)) {
  | Object(_, fields) => {
      queryFields:
        /* sort query fields alphabetically */
        fields->List.sort(({name: nameA}, {name: nameB}) =>
          String.compare(nameA, nameB)
        ),
    }
  | _ => raise(Json.Decode.DecodeError("Expected query type to be Object"))
  };
};

/* create alias so we can access schema type from nested module */
type schema = t;

module Context =
  Context.MakePair({
    type t = schema;
    let defaultValue = {queryFields: []};
  });