open Belt;

type config = {
  queryField: string,
  columns: list(string),
};

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

let buildQuery = (config: config): string => {
  let queryField = buildFieldQuery(config.queryField, config.columns);
  "{ " ++ queryField ++ " }";
};

let component = ReasonReact.statelessComponent("FielTable");

let make = (~schema: Schema.t, ~config: config, _children) => {
  ...component,
  render: _self => {
    let query = buildQuery(config);
    Js.log(query);
    /* use query as component key to force re-mount (and reset state)
       when query changes */
    <FetchQuery key=query query>
      ...{
           tableRes => {
             let json =
               tableRes |> Json.Decode.at(["data", config.queryField], x => x);
             Js.log(json);
             /* TODO: update ResultTable to display nested results */
             /* <ResultTable rowFields json />; */
             ReasonReact.null;
           }
         }
    </FetchQuery>;
  },
};