open Belt;

type state = TableConfig.t;

type action =
  | SetName(string)
  | SetQueryField(string);

let component = ReasonReact.reducerComponent("TableConfigSetup");

let make = (~schema: Schema.t, ~setConfig, _children) => {
  ...component,
  initialState: () =>
    TableConfig.{name: "", queryField: "", columns: [], schema},
  reducer: (action, state) =>
    switch (action) {
    | SetName(name) => ReasonReact.Update(TableConfig.{...state, name})
    | SetQueryField(queryField) =>
      ReasonReact.Update(TableConfig.{...state, queryField})
    },
  render: ({send, state}) =>
    <Form onSubmit={() => setConfig(state)}>
      <Layout.Row>
        <Form.Group>
          <Form.Label for_="name">
            {ReasonReact.string("Table name")}
          </Form.Label>
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
            {ReasonReact.string("Query field")}
          </Form.Label>
          <Form.Select
            id="queryField"
            options={
              schema.queryFields
              ->List.map(({name}) => Form.Select.{value: name, label: name})
            }
            value=TableConfig.(state.queryField)
            onChange={value => send(SetQueryField(value))}
          />
        </Form.Group>
      </Layout.Row>
      <Form.SubmitButton> {ReasonReact.string("Save")} </Form.SubmitButton>
    </Form>,
};