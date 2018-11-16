type t = {
  apiUrl: string,
  authHeader: string,
  table: option(TableConfig.t),
};

let encode = config =>
  Json.Encode.(
    object_([
      ("apiUrl", string(config.apiUrl)),
      ("authHeader", string(config.authHeader)),
      ("table", config.table |> nullable(TableConfig.encode)),
    ])
  );

let decode = json =>
  Json.Decode.{
    apiUrl: json |> field("apiUrl", string),
    authHeader: json |> field("authHeader", string),
    table: json |> optional(field("table", TableConfig.decode)),
  };

type context = {
  config: t,
  setConfig: t => unit,
};

module Context =
  Context.MakePair({
    type t = context;

    let defaultValue = {
      config: {
        apiUrl: "",
        authHeader: "",
        table: None,
      },
      setConfig: _config => (),
    };
  });