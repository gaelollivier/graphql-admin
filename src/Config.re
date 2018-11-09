type t = {
  apiUrl: string,
  authHeader: string,
};

let decode = json =>
  Json.Decode.{
    apiUrl: json |> field("apiUrl", string),
    authHeader: json |> field("authHeader", string),
  };