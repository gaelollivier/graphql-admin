open Belt;

let component = ReasonReact.statelessComponent("ResultTable");

let decodeRow = (displayableFields, json) =>
  Table.(
    <Row key=Json.Decode.(json |> field("id", string))>
      {
        displayableFields
        ->List.map(name =>
            <Cell key=name>
              Json.Decode.(
                (json |> optional(field(name, string)))
                ->Option.getWithDefault("")
                ->ReasonReact.string
              )
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
      ->List.keep(field => field.type_.kind == "SCALAR")
      ->List.map(field => field.name);
    let rows =
      Json.Decode.(
        json
        |> at(["data", "licenses"], array(decodeRow(displayableFields)))
      );
    Table.(
      <Table>
        <Head>
          {
            displayableFields
            ->List.map(name =>
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