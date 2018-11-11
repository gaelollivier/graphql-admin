open Belt;
open Layout;

type state = option(Config.config);

type action =
  | SetConfig(Config.config);

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
    | SetConfig(config) =>
      ReasonReact.UpdateWithSideEffects(
        Some(config),
        /* store config in local storage */
        (
          _self =>
            Dom.Storage.(
              localStorage
              |> setItem(
                   localStorageNamespace,
                   Config.encode(config)->Json.stringify,
                 )
            )
        ),
      )
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
                     <Config.Context.Provider value=config>
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
                       </FetchQuery>
                     </Config.Context.Provider>
                   }
                 }
               </Body>
             </>
         }
    </Router>,
};