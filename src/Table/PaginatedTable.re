/* Renders a table of a paginated connection
 * For now, only supported schema is:
 * field(offset, limit) { totalCount nodes { ... } }
 */

type state = {currentPage: int};

type action =
  | SetPage(int);

let component = ReasonReact.reducerComponent("FielTable");

let make = (~config: TableConfig.t, _children) => {
  ...component,
  initialState: () => {currentPage: 1},
  reducer: (action, _state) =>
    switch (action) {
    | SetPage(page) => ReasonReact.Update({currentPage: page})
    },
  render: self => {
    let {currentPage} = self.state;
    let itemsPerPage = 20;
    let (query, variables) =
      TableQuery.buildPaginatedQuery(~config, ~currentPage, ~itemsPerPage);
    /* Js.log(query); */
    /* use query as component key to force re-mount (and reset state)
       when query changes */
    <FetchQuery key={query ++ Json.stringify(variables)} query variables>
      ...{
           tableRes => {
             let json =
               tableRes
               |> Json.Decode.at(["data", config.queryField, "nodes"], x => x);
             let totalCount =
               tableRes
               |> Json.Decode.(
                    at(["data", config.queryField, "totalCount"], int)
                  );
             <>
               <ResultTable config json />
               <Pagination
                 onChange={page => self.send(SetPage(page))}
                 itemsPerPage
                 totalCount
                 currentPage
               />
             </>;
           }
         }
    </FetchQuery>;
  },
};