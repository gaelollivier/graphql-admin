open Belt;

let isPaginated = (schema: Schema.t, config: TableConfig.t): bool => {
  let rowType =
    schema.queryFields
    ->List.getBy(field => field.name == config.queryField)
    ->Option.getExn.
      typeRef;
  switch (rowType) {
  /* if the field type is a list of any kind, we dont' consider it paginated */
  | List(_)
  | NonNull(List(_)) => false
  | _ => true
  };
};

let component = ReasonReact.statelessComponent("FielTable");

let make = (~config: TableConfig.t, _children) => {
  ...component,
  render: _self =>
    <Schema.Context.Consumer>
      ...{
           schema =>
             if (isPaginated(schema, config)) {
               <PaginatedTable config />;
             } else {
               <SimpleTable config />;
             }
         }
    </Schema.Context.Consumer>,
};