type t = {
  name: string,
  queryField: string,
  columns: list(string),
};

let encode = config =>
  Json.Encode.(
    object_([
      ("name", string(config.name)),
      ("queryField", string(config.queryField)),
      ("columns", config.columns |> list(string)),
    ])
  );

let decode = json =>
  Json.Decode.{
    name: json |> field("name", string),
    queryField: json |> field("queryField", string),
    columns: json |> field("columns", list(string)),
  };