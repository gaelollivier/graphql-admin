type t = {
  apiUrl: string,
  authHeader: string,
  table: option(TableConfig.t),
};

let decode = json =>
  Json.Decode.{
    apiUrl: json |> field("apiUrl", string),
    authHeader: json |> field("authHeader", string),
    table: None,
  };

let encode = config =>
  Json.Encode.(
    object_([
      ("apiUrl", string(config.apiUrl)),
      ("authHeader", string(config.authHeader)),
    ])
  );

/* create alias so we can access config type from nested module */
type config = t;

module Context =
  Context.MakePair({
    type t = config;
    let defaultValue = {apiUrl: "", authHeader: "", table: None};
  });