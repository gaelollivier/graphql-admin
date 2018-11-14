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
           _route =>
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
                                    <Row>
                                      <Card title="Table config">
                                        <TableConfigSetup
                                          schema
                                          setConfig={_ => ()}
                                        />
                                      </Card>
                                    </Row>
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