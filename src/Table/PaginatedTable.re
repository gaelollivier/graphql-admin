open Belt;

/* Renders a table of a paginated connection
 * For now, only supported schema is:
 * field(offset, limit) { totalCount nodes { ... } }
 */

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

let buildQuery = (config: TableConfig.t): (string, Js.Json.t) => {
  let nodesField = buildFieldQuery("nodes", config.columns);
  let query =
    "query PaginatedTable($offset: Int, $limit: Int) { "
    ++ config.queryField
    ++ "(offset: $offset, limit: $limit) { totalCount "
    ++ nodesField
    ++ " } }";
  let currentPage = 1;
  let itemsPerPage = 10;
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

let component = ReasonReact.statelessComponent("FielTable");

let make = (~config: TableConfig.t, _children) => {
  ...component,
  render: _self => {
    let (query, variables) = buildQuery(config);
    Js.log(query);
    /* use query as component key to force re-mount (and reset state)
       when query changes */
    <FetchQuery key={query ++ Json.stringify(variables)} query variables>
      ...{
           tableRes => {
             let json =
               tableRes
               |> Json.Decode.at(["data", config.queryField, "nodes"], x => x);
             <>
               <ResultTable config json />
               <nav>
                 <ul className="pagination">
                   <li className="page-item">
                     <a className="page-link" href="#">
                       {ReasonReact.string("Prev")}
                     </a>
                   </li>
                   <li className="page-item">
                     <a className="page-link" href="#">
                       {ReasonReact.string("Next")}
                     </a>
                   </li>
                 </ul>
               </nav>
             </>;
           }
         }
    </FetchQuery>;
  },
};