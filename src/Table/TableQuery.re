open Belt;

/*
 * Build GraphQL query for a given field and sub-selection
 * Ex: buildFieldQuery("users", ["id", "profile.name", "profile.avatar.url"])
 * -> "users { id profile { name avatar { url } } }"
 */
let rec buildFieldQuery = (field: string, selection: list(string)): string => {
  let (subFields, _consumed) =
    /* consumed allows us to keep track of fields we already recursed */
    selection->List.reduce(("", []), ((acc, consumed), currentSelection) =>
      if (!currentSelection->String.contains('.')) {
        (acc ++ " " ++ currentSelection, consumed);
      } else {
        let currentField =
          currentSelection->String.sub(
            0,
            currentSelection->String.index('.'),
          );
        /* if already consumed, skip this selection */
        if (consumed->List.has(currentField, (==))) {
          (acc, consumed);
        } else {
          /* extract all sub-selections for current field */
          let subFields =
            selection->List.keepMap(field =>
              if (field |> Js.String.startsWith(currentField ++ ".")) {
                /* remove prefix to keep only sub-selection name */
                Some(
                  field->Js.String.substr(
                    ~from=field->String.index('.') + 1,
                  ),
                );
              } else {
                None;
              }
            );
          (
            acc ++ " " ++ buildFieldQuery(currentField, subFields),
            [currentField, ...consumed],
          );
        };
      }
    );
  field ++ " {" ++ subFields ++ " }";
};

let buildQuery = (config: TableConfig.t): string => {
  let queryField = buildFieldQuery(config.queryField, config.columns);
  "{ " ++ queryField ++ " }";
};

let buildPaginatedQuery =
    (~config: TableConfig.t, ~currentPage: int, ~itemsPerPage: int)
    : (string, Js.Json.t) => {
  let nodesFields =
    /* Automatically add _id selection if not included */
    switch (config.columns->List.getBy(column => column == "_id")) {
    | Some(_) => config.columns
    | None => ["_id", ...config.columns]
    };
  let nodesField = buildFieldQuery("nodes", nodesFields);
  let query =
    "query PaginatedTable($offset: Int, $limit: Int) { "
    ++ config.queryField
    ++ "(offset: $offset, limit: $limit) { totalCount "
    ++ nodesField
    ++ " } }";
  let variables = Js.Dict.empty();
  Js.Dict.set(
    variables,
    "offset",
    Js.Json.number(float_of_int((currentPage - 1) * itemsPerPage)),
  );
  Js.Dict.set(
    variables,
    "limit",
    Js.Json.number(float_of_int(itemsPerPage)),
  );
  (query, Js.Json.object_(variables));
};