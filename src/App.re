open Belt;
open Layout;

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self =>
    <Router>
      ...{
           route =>
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
                                    <SidebarItem url={"/#" ++ name} key=name>
                                      {ReasonReact.string(name)}
                                    </SidebarItem>
                                  )
                                ->List.toArray
                                ->ReasonReact.array
                              }
                            </Sidebar>
                            <Content>
                              {
                                switch (route) {
                                | _ =>
                                  let queryField = "accounts";
                                  let config =
                                    TableConfig.{
                                      queryField,
                                      columns: [
                                        "_id",
                                        "email",
                                        "profile.firstname",
                                      ],
                                      schema,
                                    };
                                  <Row>
                                    <Card title=queryField>
                                      <FieldTable config />
                                    </Card>
                                  </Row>;
                                }
                              }
                            </Content>
                          </>;
                        }
                      }
                 </FetchQuery>
               </Body>
             </>
         }
    </Router>,
};