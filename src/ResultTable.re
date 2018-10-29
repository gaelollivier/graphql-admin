open Belt;

let component = ReasonReact.statelessComponent("ResultTable");

let decodeRow = (displayableFields: list(Schema.field), json) =>
  Table.(
    <Row key=Json.Decode.(json |> field("id", string))>
      {
        displayableFields
        ->List.map(({name, type_}) =>
            <Cell key=name>
              {(json |> Schema.decodeField(type_, name))->ReasonReact.string}
            </Cell>
          )
        ->List.toArray
        ->ReasonReact.array
      }
    </Row>
  );

let make = (~json: Js.Json.t, ~rowType: Schema.type_, _children) => {
  ...component,
  render: _self => {
    let displayableFields =
      rowType.fields
      ->Option.getExn
      ->List.keep(field => field.type_->Schema.isDisplayable);
    let rows = Json.Decode.(json |> array(decodeRow(displayableFields)));
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