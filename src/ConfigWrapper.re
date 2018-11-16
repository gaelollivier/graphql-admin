open Belt;

/* Top-level app wrapper to retrieve/store config from local storage.
   Displays setup page if no config is available */

type state = option(Config.t);

type action =
  | SetConfig(Config.t);

let localStorageNamespace = "graphql-admin.config";

let storeConfig = config =>
  Dom.Storage.(
    localStorage
    |> setItem(localStorageNamespace, Config.encode(config)->Json.stringify)
  );

let retrieveConfig = () =>
  switch (Dom.Storage.(localStorage |> getItem(localStorageNamespace))) {
  | Some(storedConfig) =>
    Json.parse(storedConfig)->Option.map(Config.decode)
  | None => None
  };

let component = ReasonReact.reducerComponent("Config.Wrapper");

let make = children => {
  ...component,
  initialState: () => retrieveConfig(),
  reducer: (action, _state) =>
    switch (action) {
    | SetConfig(config) =>
      ReasonReact.UpdateWithSideEffects(
        Some(config),
        (_self => storeConfig(config)),
      )
    },
  render: ({state, send}) => {
    let setConfig = newConfig => send(SetConfig(newConfig));
    switch (state) {
    | None => <ApiConfigSetup setConfig />
    | Some(config) =>
      <Config.Context.Provider value={config, setConfig}>
        ...children
      </Config.Context.Provider>
    };
  },
};