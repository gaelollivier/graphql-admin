type route =
  | AddView
  | View(string);

type action =
  | SetRoute(route);

let component = ReasonReact.reducerComponent("Router");

let matchRoute = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | ["add-view"] => AddView
  | ["view", view] => View(view)
  | _ => AddView
  };

let make = children => {
  ...component,
  initialState: () =>
    matchRoute(ReasonReact.Router.dangerouslyGetInitialUrl()),
  reducer: (action, _state) =>
    switch (action) {
    | SetRoute(route) => ReasonReact.Update(route)
    },
  didMount: self => {
    let watcherID =
      ReasonReact.Router.watchUrl(url => {
        Js.log(url);
        self.send(SetRoute(matchRoute(url)));
      });
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
  },
  render: self => children(self.state),
};