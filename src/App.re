open Belt;
open Layout;

type state = option(Config.t);

type action =
  | SetConfig(Config.t);

let component = ReasonReact.reducerComponent("App");

let localStorageNamespace = "graphql-admin";

let make = _children => {
  ...component,
  initialState: () =>
    switch (Dom.Storage.(localStorage |> getItem(localStorageNamespace))) {
    | Some(storedConfig) =>
      Json.parse(storedConfig)->Option.map(Config.decode)
    | None => None
    },
  reducer: (action, _state) =>
    switch (action) {
    | SetConfig(config) => ReasonReact.Update(Some(config))
    },
  render: self =>
    <Router>
      ...{
           route =>
             <>
               <Header> {ReasonReact.string("GraphQL Admin")} </Header>
               <Body>
                 {
                   switch (self.state) {
                   | None =>
                     <ApiConfigSetup
                       setConfig=(config => self.send(SetConfig(config)))
                     />
                   | Some(config) =>
                     Js.log(config);
                     /* TODO: ConfigProvider */
                     <FetchQuery query=Schema.introspectionQuery>
                       ...(
                            introspectionRes => {
                              let schema =
                                Schema.decodeIntrospectionQuery(
                                  introspectionRes,
                                );
                              <>
                                <Sidebar>
                                  {
                                    schema.queryFields
                                    ->List.map(({name}) =>
                                        <SidebarItem
                                          url={"/#" ++ name} key=name>
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
                          )
                     </FetchQuery>;
                   }
                 }
               </Body>
             </>
         }
    </Router>,
};