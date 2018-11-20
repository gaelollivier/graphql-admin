open Layout;
open Belt;

let component = ReasonReact.statelessComponent("AddView");

let make = _children => {
  ...component,
  render: _self =>
    <Row>
      <Config.Context.Consumer>
        ...{
             ({config, setConfig}: Config.context) =>
               <Card title="Create a new view">
                 <TableConfigSetup
                   setConfig={
                     tableConfig => {
                       setConfig({
                         ...config,
                         tables: List.concat(config.tables, [tableConfig]),
                       });
                       ReasonReact.Router.push("/view/" ++ tableConfig.name);
                     }
                   }
                 />
               </Card>
           }
      </Config.Context.Consumer>
    </Row>,
};