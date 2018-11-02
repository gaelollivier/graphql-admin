open Belt;

let component = ReasonReact.statelessComponent("ResultTable");

let decodeRow = (displayableFields: list(Schema.field), json) =>
  Table.(
    <Row key=Json.Decode.(json |> field("id", string))>
      {
        displayableFields
        ->List.map(({name, typeRef}) =>
            <Cell key=name>
              {
                (json |> Schema.decodeField(typeRef, name))->ReasonReact.string
              }
            </Cell>
          )
        ->List.toArray
        ->ReasonReact.array
      }
    </Row>
  );

let make = (~rowFields: list(Schema.field), ~json: Js.Json.t, _children) => {
  ...component,
  render: _self => {
    let displayableFields =
      rowFields->List.keep(field => field.typeRef->Schema.isDisplayable);
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