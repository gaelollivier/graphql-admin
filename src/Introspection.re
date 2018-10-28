open Belt;

let query = "query IntrospectionQuery {
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

type graphQLField = {
  name: string,
  type_: graphQLType,
}
and graphQLType = {
  name: option(string),
  kind: string,
  fields: option(list(graphQLField)),
  ofType: option(graphQLType),
};

type graphQLSchema = {
  queryFields: list(graphQLField),
  types: Map.String.t(graphQLType),
};

let rec decodeField = json =>
  Json.Decode.{
    name: json |> field("name", string),
    type_: json |> field("type", decodeType),
  }
and decodeType = json =>
  Json.Decode.{
    name: json |> optional(field("name", string)),
    kind: json |> field("kind", string),
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