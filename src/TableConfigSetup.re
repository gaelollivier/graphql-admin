open Belt;

let str = ReasonReact.string;

module FieldsList = {
  /* state is list of expanded fields */
  type state = list(string);

  type action =
    | Toggle(string);

  let component = ReasonReact.reducerComponent("TableConfigSetup.FieldsList");

  let rec make = (~fields: list(Schema.field), _children) => {
    ...component,
    initialState: () => ([]: state),
    reducer: (action, state) =>
      switch (action) {
      | Toggle(toggleField) =>
        switch (state->List.getBy(field => field == toggleField)) {
        | Some(_) =>
          ReasonReact.Update(state->List.keep(field => field != toggleField))
        | None => ReasonReact.Update([toggleField, ...state])
        }
      },
    render: ({state: expanded, send}) =>
      <ul>
        {
          fields
          ->List.map(field =>
              <li key={field.name}>
                {
                  switch (field.typeRef->Schema.getReferencedTypeExn) {
                  | Schema.Object(name, fields) =>
                    <>
                      <a
                        onClick=(_ => send(Toggle(name)))
                        style={ReactDOMRe.Style.make(~cursor="pointer", ())}>
                        {str(field.name)}
                      </a>
                      {
                        switch (expanded->List.getBy(field => field == name)) {
                        | None => ReasonReact.null
                        /* for recusion, we can't use JSX so we call `make` manually */
                        | Some(_) => ReasonReact.element(make(~fields, [||]))
                        }
                      }
                    </>
                  | _ =>
                    <label htmlFor={field.name}>
                      <input id={field.name} type_="checkbox" />
                      {str(" " ++ field.name)}
                    </label>
                  }
                }
              </li>
            )
          ->List.toArray
          ->ReasonReact.array
        }
      </ul>,
  };
};

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
          <Form.Label for_="queryField"> {str("Query field")} </Form.Label>
          <Form.Select
            id="queryField"
            options={
              schema.queryFields
              ->List.map(({name}) => Form.Select.{value: name, label: name})
              ->List.add(Form.Select.{value: "", label: "Select field"})
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
            switch (FieldTable.isPaginated(state)) {
            | false =>
              field.typeRef->Schema.getReferencedTypeExn->Schema.getFieldsExn
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
              <FieldsList fields />
            </Form.Group>
          </Layout.Row>;
        };
      }
      <Form.SubmitButton> {str("Save")} </Form.SubmitButton>
    </Form>,
};