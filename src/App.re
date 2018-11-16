open Layout;

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self =>
    <Router>
      ...{
           _route =>
             <>
               <Header> {ReasonReact.string("GraphQL Admin")} </Header>
               <Body>
                 <ConfigWrapper>
                   <FetchSchema>
                     <> <Sidebar /> <Content> <HomePage /> </Content> </>
                   </FetchSchema>
                 </ConfigWrapper>
               </Body>
             </>
         }
    </Router>,
};