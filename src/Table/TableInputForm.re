open Belt;

module ArgumentList = {
  let component = ReasonReact.statelessComponent("ArgumentList");

  let rec make = (~parentField="", ~args: list(Schema.arg), _children) => {
    ...component,
    render: _self =>
      args
      ->List.map(arg =>
          switch (arg.typeRef->Schema.getReferencedTypeExn) {
          | Schema.InputObject(_name, args) =>
            ReasonReact.element(
              ~key=arg.name,
              make(~args, ~parentField=parentField ++ arg.name ++ ".", [||]),
            )
          | _ =>
            <Form.Group key={arg.name}>
              <Form.InputGroup>
                <Form.InputGroupText>
                  {ReasonReact.string(parentField ++ arg.name)}
                </Form.InputGroupText>
                <Form.TextInput id={arg.name} value="" onChange=(_ => ()) />
              </Form.InputGroup>
            </Form.Group>
          }
        )
      ->ReactUtils.list,
  };
};

let component = ReasonReact.statelessComponent("TableInputForm");

let make = (~config: TableConfig.t, _children) => {
  ...component,
  render: _self =>
    <Schema.Context.Consumer>
      ...{
           schema => {
             let args =
               schema.queryFields
               ->List.getBy(field => field.name == config.queryField)
               ->Option.getExn.
                 args
               /* filter-out fields use for pagination/sorting */
               ->List.keep(arg =>
                   switch (arg.name) {
                   | "offset"
                   | "limit"
                   | "orderBy" => false
                   | _ => true
                   }
                 );
             <Grid.Row>
               <Grid.Cell size=5>
                 <Form onSubmit={() => ()}> <ArgumentList args /> </Form>
               </Grid.Cell>
             </Grid.Row>;
           }
         }
    </Schema.Context.Consumer>,
};