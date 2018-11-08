let component = ReasonReact.statelessComponent("FielTable");

let make = (~config: TableConfig.t, _children) => {
  ...component,
  render: _self => {
    let query = TableQuery.buildQuery(config);
    Js.log(query);
    /* use query as component key to force re-mount (and reset state)
       when query changes */
    <FetchQuery key=query query>
      ...{
           tableRes => {
             let json =
               tableRes |> Json.Decode.at(["data", config.queryField], x => x);
             <ResultTable config json />;
           }
         }
    </FetchQuery>;
  },
};