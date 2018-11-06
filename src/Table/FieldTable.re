open Belt;

let isPaginated = (config: TableConfig.t): bool => {
  let rowType =
    config.schema.queryFields
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
    if (isPaginated(config)) {
      <PaginatedTable config />;
    } else {
      <SimpleTable config />;
    },
};