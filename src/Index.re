let authToken = "Bearer ";

let _ = {
  let payload = Js.Dict.empty();
  Js.Dict.set(
    payload,
    "query",
    Js.Json.string("{
    contacts {
      totalCount
    }
  }"),
  );
  Js.Promise.(
    Fetch.fetchWithInit(
      "http://localhost:5000/graphql",
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=
          Fetch.BodyInit.make(Js.Json.stringify(Js.Json.object_(payload))),
        ~headers=
          Fetch.HeadersInit.make({
            "Content-Type": "application/json",
            "Authorization": authToken,
          }),
        (),
      ),
    )
    |> then_(Fetch.Response.json)
    |> then_(res => {
         Js.log(res);
         Js.Promise.resolve();
       })
  );
};