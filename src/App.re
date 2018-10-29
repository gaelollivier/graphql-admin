open Belt;
open Layout;

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self =>
    <>
      <Header> {ReasonReact.string("GraphQL Admin")} </Header>
      <Body>
        <FetchQuery query=Schema.introspectionQuery>
          ...{
               introspectionRes => {
                 let schema =
                   Schema.decodeIntrospectionQuery(introspectionRes);
                 <>
                   <Sidebar>
                     {
                       schema.queryFields
                       ->List.map(({name}) =>
                           <SidebarItem key=name>
                             {ReasonReact.string(name)}
                           </SidebarItem>
                         )
                       ->List.toArray
                       ->ReasonReact.array
                     }
                   </Sidebar>
                   <Content>
                     <Row>
                       <Card title="Table">
                         <FieldTable
                           schema
                           fieldName="marketplaceCategories"
                         />
                       </Card>
                     </Row>
                   </Content>
                 </>;
               }
             }
        </FetchQuery>
      </Body>
    </>,
};