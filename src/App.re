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
                                | Router.Index =>
                                  ReasonReact.string(
                                    "Select query field in sidebar",
                                  )
                                | Router.QueryField(_field) =>
                                  let config =
                                    FieldTable.{
                                      queryField: "licenses",
                                      columns: [
                                        "id",
                                        "name",
                                        "nickname",
                                        "conditions.label",
                                        "conditions.description",
                                      ],
                                    };
                                  <Row>
                                    <Card title="Table">
                                      <FieldTable schema config />
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