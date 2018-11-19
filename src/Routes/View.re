open Layout;
open Belt;

let component = ReasonReact.statelessComponent("View");

let make = (~name, _children) => {
  ...component,
  render: _self =>
    <Row>
      <Config.Context.Consumer>
        ...{
             ({config}: Config.context) => {
               let tableConfig =
                 config.tables->List.getBy(table => table.name == name);
               switch (tableConfig) {
               | None => ReasonReact.string("View not found")
               | Some(tableConfig) =>
                 <Card title={tableConfig.queryField}>
                   <FieldTable config=tableConfig />
                 </Card>
               };
             }
           }
      </Config.Context.Consumer>
    </Row>,
};