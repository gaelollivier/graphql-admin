open Belt;

/* For a given query field, returns the type of result items
   For now, only support list of objects */
let getRowType =
    (schema: Schema.t, fieldName: string): Result.t(Schema.type_, string) => {
  let fieldType =
    schema.queryFields
    ->List.getBy(field => field.name == fieldName)
    ->Option.getExn.
      type_
    ->Schema.unwrapNonNull;
  switch (fieldType) {
  | {kind: Schema.List, ofType: Some(itemType)} =>
    let typeRef = itemType->Schema.unwrapNonNull;
    switch (typeRef) {
    | {kind: Schema.Object} =>
      Result.Ok(schema.types->Map.String.getExn(typeRef.name->Option.getExn))
    | _ => Result.Error("Items should be objects")
    };
  | _ => Result.Error("Only support lists")
  };
};

let component = ReasonReact.statelessComponent("FielTable");

let make = (~schema: Schema.t, ~fieldName: string, _children) => {
  ...component,
  render: _self =>
    switch (getRowType(schema, fieldName)) {
    | Result.Error(err) => ReasonReact.string("Unsupported type: " ++ err)
    | Result.Ok(rowType) =>
      let queryFields =
        rowType.fields
        ->Option.getExn
        ->List.keep(field => field.type_->Schema.isDisplayable)
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
               <ResultTable json rowType />;
             }
           )
      </FetchQuery>;
    },
};