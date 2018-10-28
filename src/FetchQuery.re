let fetchQuery = query => {
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "query", Js.Json.string(query));

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
  );
};

type state = option(Js.Json.t);

type action =
  | SetResult(Js.Json.t);

let component = ReasonReact.reducerComponent("FetchQuery");

let make = (~query: string, children) => {
  ...component,
  initialState: () => None,
  reducer: (action, _state) =>
    switch (action) {
    | SetResult(res) => ReasonReact.Update(Some(res))
    },
  didMount: self =>
    Js.Promise.(
      fetchQuery(query)
      |> then_(res => {
           self.send(SetResult(res));
           resolve();
         })
    )
    |> ignore,
  render: self =>
    switch (self.state) {
    | Some(res) => children(res)
    | None => ReasonReact.string("Loading...")
    },
};