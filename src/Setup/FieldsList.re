open Belt;
let str = ReasonReact.string;

/* state is list of expanded fields */
type state = list(string);

type action =
  | Toggle(string);

let component = ReasonReact.reducerComponent("TableConfigSetup.FieldsList");

let rec make =
        (
          ~fields: list(Schema.field),
          ~toggleField: string => unit,
          ~selection: list(string),
          ~path="",
          _children,
        ) => {
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
        ->List.map(field => {
            let fullPath = (path == "" ? "" : path ++ ".") ++ field.name;
            <li key=fullPath>
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
                      | Some(_) =>
                        ReasonReact.element(
                          make(
                            ~fields,
                            ~toggleField,
                            ~selection,
                            ~path=fullPath,
                            [||],
                          ),
                        )
                      }
                    }
                  </>
                | _ =>
                  <label htmlFor={"field-" ++ fullPath}>
                    <input
                      id={"field-" ++ fullPath}
                      type_="checkbox"
                      onChange=(_ => toggleField(fullPath))
                      checked={
                        switch (
                          selection->List.getBy(column => column === fullPath)
                        ) {
                        | Some(_) => true
                        | None => false
                        }
                      }
                    />
                    {str(" " ++ field.name)}
                  </label>
                }
              }
            </li>;
          })
        ->List.toArray
        ->ReasonReact.array
      }
    </ul>,
};