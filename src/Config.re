type t = {
  apiUrl: string,
  authHeader: string,
  tables: list(TableConfig.t),
};

let encode = config =>
  Json.Encode.(
    object_([
      ("apiUrl", string(config.apiUrl)),
      ("authHeader", string(config.authHeader)),
      ("tables", config.tables |> list(TableConfig.encode)),
    ])
  );

let decode = json =>
  Json.Decode.{
    apiUrl: json |> field("apiUrl", string),
    authHeader: json |> field("authHeader", string),
    tables: json |> field("tables", list(TableConfig.decode)),
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
        tables: [],
      },
      setConfig: _config => (),
    };
  });