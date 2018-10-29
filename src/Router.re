type route =
  | Index
  | QueryField(string);

type action =
  | SetRoute(route);

let component = ReasonReact.reducerComponent("Router");

let make = children => {
  ...component,
  initialState: () => Index,
  reducer: (action, _state) =>
    switch (action) {
    | SetRoute(route) => ReasonReact.Update(route)
    },
  didMount: self => {
    let watcherID =
      ReasonReact.Router.watchUrl(url =>
        switch (url.hash) {
        | str when String.length(str) > 0 =>
          self.send(SetRoute(QueryField(str)))
        | _ => self.send(SetRoute(Index))
        }
      );
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
  },
  render: self => children(self.state),
};