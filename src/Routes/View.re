open Layout;
open Belt;

let component = ReasonReact.statelessComponent("View");

let updateViewConfig =
    (oldConfig: Config.t, oldName: string, view: TableConfig.t) => {
  ...oldConfig,
  tables:
    oldConfig.tables
    ->List.keep(config => config.name !== oldName)
    ->List.concat([view]),
};

let deleteViewConfig = (oldConfig: Config.t, view: TableConfig.t) => {
  ...oldConfig,
  tables: oldConfig.tables->List.keep(config => config.name !== view.name),
};

let make = (~name, ~path, _children) => {
  ...component,
  render: _self =>
    <Row>
      <Config.Context.Consumer>
        ...{
             ({config, setConfig}: Config.context) => {
               let tableConfig =
                 config.tables->List.getBy(table => table.name == name);
               switch (tableConfig) {
               | None => ReasonReact.string("View not found")
               | Some(tableConfig) =>
                 <>
                   {
                     switch (path) {
                     | ["config"] =>
                       <Card
                         title="View configuration"
                         closeUrl={"/view/" ++ tableConfig.name}>
                         <TableConfigSetup
                           initialConfig=tableConfig
                           setConfig=(
                             newConfig => {
                               setConfig(
                                 updateViewConfig(
                                   config,
                                   tableConfig.name,
                                   newConfig,
                                 ),
                               );
                               ReasonReact.Router.push(
                                 "/view/" ++ newConfig.name,
                               );
                             }
                           )
                           deleteView=(
                             () => {
                               setConfig(
                                 deleteViewConfig(config, tableConfig),
                               );
                               ReasonReact.Router.push("/");
                             }
                           )
                         />
                       </Card>
                     | _ => ReasonReact.null
                     }
                   }
                   <Card
                     title={tableConfig.name}
                     configUrl={"/view/" ++ tableConfig.name ++ "/config"}>
                     <FieldTable config=tableConfig />
                   </Card>
                 </>
               };
             }
           }
      </Config.Context.Consumer>
    </Row>,
};