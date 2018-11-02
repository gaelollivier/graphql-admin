open Belt;

/* For a given query field, returns the list of fields within the result items
   For now, only support list of objects */
let getRowFields =
    (schema: Schema.t, fieldName: string)
    : Result.t(list(Schema.field), string) => {
  let fieldType =
    schema.queryFields
    ->List.getBy(field => field.name == fieldName)
    ->Option.getExn.
      typeRef
    ->Schema.unwrapNonNull;
  switch (fieldType) {
  | List(itemType) =>
    let typeRef = itemType->Schema.unwrapNonNull;
    switch (typeRef) {
    | NonNull(_)
    | List(_) => Result.Error("Invalid field type")
    | Type(type_) =>
      switch (Lazy.force(type_)) {
      | Object(_, fields) => Result.Ok(fields)
      | _ => Result.Error("Items should be objects")
      }
    };
  | _ => Result.Error("Only support lists")
  };
};

let component = ReasonReact.statelessComponent("FielTable");

let make = (~schema: Schema.t, ~fieldName: string, _children) => {
  ...component,
  render: _self =>
    switch (getRowFields(schema, fieldName)) {
    | Result.Error(err) => ReasonReact.string("Unsupported type: " ++ err)
    | Result.Ok(rowFields) =>
      let queryFields =
        rowFields
        ->List.keep(field => field.typeRef->Schema.isDisplayable)
        ->List.map(field => field.name)
        |> String.concat(" ");
      /* use fieldName as component key to force re-mount (and reset state)
         when field name changes */
      <FetchQuery
        key=fieldName
        query={"{ " ++ fieldName ++ " { " ++ queryFields ++ " } }"}>
        ...(
             tableRes => {
               let json =
                 tableRes |> Json.Decode.at(["data", fieldName], x => x);
               <ResultTable rowFields json />;
             }
           )
      </FetchQuery>;
    },
};