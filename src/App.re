open Belt;
open Layout;

/* get type of each individual rows for a given query field */
let getRowFieldsExn = (schema: Schema.t, queryField) => {
  let rowType =
    schema.queryFields
    ->List.getBy(field => field.name == queryField)
    ->Option.getExn.
      typeRef
    /* unwrap list/non null
       NOTE: we don't support connection objects (for pagination), only simple lists */
    ->Schema.getReferencedTypeExn;
  switch (rowType) {
  | Schema.Object(_, fields) => fields
  | _ => raise(Invalid_argument("Expected row type to be object"))
  };
};

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
                                  let queryField = "licenses";
                                  let config =
                                    TableConfig.{
                                      queryField,
                                      columns: [
                                        "id",
                                        "name",
                                        "nickname",
                                        "conditions.label",
                                        "conditions.description",
                                      ],
                                      rowFields:
                                        getRowFieldsExn(schema, queryField),
                                    };
                                  <Row>
                                    <Card title="Table">
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