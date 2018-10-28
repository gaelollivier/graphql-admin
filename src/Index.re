open Belt;

let _ = {
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "query", Js.Json.string(Introspection.query));

  Js.Promise.(
    Fetch.fetchWithInit(
      "https://api.github.com/graphql",
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=
          Fetch.BodyInit.make(Js.Json.stringify(Js.Json.object_(payload))),
        ~headers=
          Fetch.HeadersInit.make({
            "Content-Type": "application/json",
            "Authorization": Auth.token,
          }),
        (),
      ),
    )
    |> then_(Fetch.Response.json)
    |> then_(res => {
         let schema = Introspection.decodeIntrospectionQuery(res);
         schema.queryFields->List.forEach(field => Js.log(field.name));
         Js.Promise.resolve();
       })
  );
};

ReactDOMRe.renderToElementWithId(<App />, "root");