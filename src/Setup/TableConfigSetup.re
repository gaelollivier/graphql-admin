open Belt;
let str = ReasonReact.string;

type state = TableConfig.t;

type action =
  | SetName(string)
  | SetQueryField(string)
  | ToggleColumn(string);

let component = ReasonReact.reducerComponent("TableConfigSetup");

let make = (~setConfig, ~initialConfig=?, ~deleteView=?, _children) => {
  ...component,
  initialState: () =>
    switch (initialConfig) {
    | Some(config) => config
    | None => TableConfig.{name: "", queryField: "", columns: []}
    },
  reducer: (action, state) =>
    switch (action) {
    | SetName(name) => ReasonReact.Update(TableConfig.{...state, name})
    | SetQueryField(queryField) =>
      ReasonReact.Update(TableConfig.{...state, queryField})
    | ToggleColumn(toggleColumn) =>
      switch (state.columns->List.getBy(column => column == toggleColumn)) {
      | Some(_) =>
        ReasonReact.Update({
          ...state,
          columns: state.columns->List.keep(column => column != toggleColumn),
        })
      | None =>
        ReasonReact.Update({
          ...state,
          columns: List.concat(state.columns, [toggleColumn]),
        })
      }
    },
  render: ({send, state}) =>
    <Schema.Context.Consumer>
      ...{
           schema =>
             <Form onSubmit={() => setConfig(state)}>
               <Layout.Row>
                 <Form.Group>
                   <Form.Label for_="name"> {str("Table name")} </Form.Label>
                   <Form.TextInput
                     id="name"
                     placeholder="Users"
                     value=TableConfig.(state.name)
                     onChange={value => send(SetName(value))}
                   />
                 </Form.Group>
               </Layout.Row>
               <Layout.Row>
                 <Form.Group>
                   <Form.Label for_="queryField">
                     {str("Query field")}
                   </Form.Label>
                   <Form.Select
                     id="queryField"
                     options={
                       schema.queryFields
                       ->List.map(({name}) =>
                           Form.Select.{value: name, label: name}
                         )
                       ->List.add(
                           Form.Select.{value: "", label: "Select field"},
                         )
                     }
                     value=TableConfig.(state.queryField)
                     onChange={value => send(SetQueryField(value))}
                   />
                 </Form.Group>
               </Layout.Row>
               {
                 let field =
                   schema.queryFields
                   ->List.getBy(field => field.name === state.queryField);
                 switch (field) {
                 | None => ReasonReact.null
                 | Some(field) =>
                   let fields =
                     switch (FieldTable.isPaginated(schema, state)) {
                     | false =>
                       field.typeRef
                       ->Schema.getReferencedTypeExn
                       ->Schema.getFieldsExn
                     | true =>
                       field.typeRef
                       ->Schema.getReferencedTypeExn
                       ->Schema.getFieldsExn
                       /* for paginated field, needs to retrieve "nodes" types */
                       ->List.getBy(field => field.name === "nodes")
                       ->Option.getExn.
                         typeRef
                       ->Schema.getReferencedTypeExn
                       ->Schema.getFieldsExn
                     };
                   <Layout.Row>
                     <Form.Group>
                       <Form.Label> {str("Columns")} </Form.Label>
                       <FieldsList
                         fields
                         toggleField=(field => send(ToggleColumn(field)))
                         selection={state.columns}
                       />
                     </Form.Group>
                   </Layout.Row>;
                 };
               }
               <Form.SubmitButton> {str("Save")} </Form.SubmitButton>
               {
                 switch (deleteView) {
                 | None => ReasonReact.null
                 | Some(delete) =>
                   <Form.DangerButton onClick=delete>
                     {str("Delete view")}
                   </Form.DangerButton>
                 }
               }
             </Form>
         }
    </Schema.Context.Consumer>,
};