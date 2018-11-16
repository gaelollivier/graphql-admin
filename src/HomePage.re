open Layout;

let component = ReasonReact.statelessComponent("HomePage");

let make = _children => {
  ...component,
  render: _self =>
    <Row>
      <Config.Context.Consumer>
        ...{
             ({config, setConfig}: Config.context) =>
               switch (config.table) {
               | None =>
                 <Card title="Table config">
                   <TableConfigSetup
                     setConfig=(
                       tableConfig =>
                         setConfig({...config, table: Some(tableConfig)})
                     )
                   />
                 </Card>
               | Some(tableConfig) =>
                 <Card title={tableConfig.queryField}>
                   <FieldTable config=tableConfig />
                 </Card>
               }
           }
      </Config.Context.Consumer>
    </Row>,
};