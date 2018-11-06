open Belt;

let component = ReasonReact.statelessComponent("ResultTable");

let rec renderValue = (column: string, json) =>
  if (column->String.contains('.')) {
    let subField = column->String.sub(0, column->String.index('.'));
    let subSelection =
      column->Js.String.substr(~from=column->String.index('.') + 1);
    let subValue = Json.Decode.(json |> field(subField, x => x));
    switch (Js.Json.classify(subValue)) {
    | Js.Json.JSONArray(_) =>
      let values = Json.Decode.(subValue |> list(renderValue(subSelection)));
      <ul>
        {
          values
          ->List.mapWithIndex((i, value) =>
              <li key={string_of_int(i)}> value </li>
            )
          ->List.toArray
          ->ReasonReact.array
        }
      </ul>;
    | _ => ReasonReact.null
    };
  } else {
    let value = Json.Decode.(json |> field(column, x => x));
    switch (Js.Json.classify(value)) {
    | Js.Json.JSONString(str) => ReasonReact.string(str)
    | Js.Json.JSONNull => ReasonReact.string("NULL")
    | _ => ReasonReact.string(Json.stringify(value))
    };
  };

let renderRow = (config: TableConfig.t, json) =>
  Table.(
    <Row key=Json.Decode.(json |> field("id", string))>
      {
        config.columns
        ->List.map(column =>
            <Cell key=column> {json |> renderValue(column)} </Cell>
          )
        ->List.toArray
        ->ReasonReact.array
      }
    </Row>
  );

let make = (~config: TableConfig.t, ~json: Js.Json.t, _children) => {
  ...component,
  render: _self =>
    Table.(
      <Table>
        <Head>
          {
            config.columns
            ->List.map(column =>
                <HeadColumn key=column>
                  {ReasonReact.string(column)}
                </HeadColumn>
              )
            ->List.toArray
            ->ReasonReact.array
          }
        </Head>
        <Rows> ...Json.Decode.(json |> array(renderRow(config))) </Rows>
      </Table>
    ),
};