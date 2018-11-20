let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self =>
    <Router>
      ...{
           route =>
             <>
               <Layout.Header>
                 {ReasonReact.string("GraphQL Admin")}
               </Layout.Header>
               <Layout.Body>
                 <ConfigWrapper>
                   <FetchSchema>
                     <>
                       <Sidebar />
                       <Layout.Content>
                         {
                           switch (route) {
                           | Router.AddView => <AddView />
                           | Router.View(name, path) => <View name path />
                           }
                         }
                       </Layout.Content>
                     </>
                   </FetchSchema>
                 </ConfigWrapper>
               </Layout.Body>
             </>
         }
    </Router>,
};