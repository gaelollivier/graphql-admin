let fetchQuery = (query, variables) => {
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "query", Js.Json.string(query));

  switch (variables) {
  | None => ()
  | Some(variables) => Js.Dict.set(payload, "variables", variables)
  };

  Js.Promise.(
    Fetch.fetchWithInit(
      Auth.apiUrl,
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

let make = (~query: string, ~variables: option(Js.Json.t)=?, children) => {
  ...component,
  initialState: () => None,
  reducer: (action, _state) =>
    switch (action) {
    | SetResult(res) => ReasonReact.Update(Some(res))
    },
  didMount: self =>
    Js.Promise.(
      fetchQuery(query, variables)
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