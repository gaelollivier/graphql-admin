type t = {
  apiUrl: string,
  authHeader: string,
};

let decode = json =>
  Json.Decode.{
    apiUrl: json |> field("apiUrl", string),
    authHeader: json |> field("authHeader", string),
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
    let defaultValue = {apiUrl: "", authHeader: ""};
  });