open Belt;

let component = ReasonReact.statelessComponent("Sidebar");

let make = _children => {
  ...component,
  render: _self =>
    <Layout.Row>
      <Config.Context.Consumer>
        ...{
             ({config}: Config.context) =>
               <Layout.Sidebar>
                 {
                   config.tables
                   ->List.map(tableConfig =>
                       <Layout.SidebarItem
                         key={tableConfig.name}
                         url={"/view/" ++ tableConfig.name}>
                         {ReasonReact.string(tableConfig.name)}
                       </Layout.SidebarItem>
                     )
                   ->List.toArray
                   ->ReasonReact.array
                 }
                 <Layout.SidebarItem url="/add-view">
                   {ReasonReact.string("+ Add view")}
                 </Layout.SidebarItem>
               </Layout.Sidebar>
           }
      </Config.Context.Consumer>
    </Layout.Row>,
};