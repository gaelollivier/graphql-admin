open Belt;

let component = ReasonReact.statelessComponent("ResultTable");

let decodeRow =
    (schema: Schema.t, displayableFields: list(Schema.field), json) =>
  Table.(
    <Row key=Json.Decode.(json |> field("id", string))>
      {
        displayableFields
        ->List.map(({name, typeRef}) =>
            <Cell key=name>
              {
                (json |> Schema.decodeField(schema, typeRef, name))
                ->ReasonReact.string
              }
            </Cell>
          )
        ->List.toArray
        ->ReasonReact.array
      }
    </Row>
  );

let make =
    (~schema: Schema.t, ~rowType: Schema.type_, ~json: Js.Json.t, _children) => {
  ...component,
  render: _self => {
    let displayableFields =
      rowType.fields
      ->Option.getExn
      ->List.keep(field => field.type_->Schema.isDisplayable);
    let rows =
      Json.Decode.(json |> array(decodeRow(schema, displayableFields)));
    Table.(
      <Table>
        <Head>
          {
            displayableFields
            ->List.map(({name}) =>
                <HeadColumn key=name> {ReasonReact.string(name)} </HeadColumn>
              )
            ->List.toArray
            ->ReasonReact.array
          }
        </Head>
        <Rows> ...rows </Rows>
      </Table>
    );
  },
};