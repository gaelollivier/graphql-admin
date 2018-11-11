type state = Config.config;

type action =
  | SetUrl(string)
  | SetAuthHeader(string);

let component = ReasonReact.reducerComponent("ConfigPage");

let make = (~setConfig, _children) => {
  ...component,
  initialState: () => Config.{apiUrl: "", authHeader: ""},
  reducer: (action, state) =>
    switch (action) {
    | SetUrl(apiUrl) => ReasonReact.Update(Config.{...state, apiUrl})
    | SetAuthHeader(authHeader) =>
      ReasonReact.Update(Config.{...state, authHeader})
    },
  render: ({send, state}) =>
    <div
      className="container-fluid"
      style={ReactDOMRe.Style.make(~marginTop="20px", ())}>
      <div className="animated fadeIn">
        <Layout.Row>
          <Layout.Card title="Configuration">
            <Form onSubmit={() => setConfig(state)}>
              <Layout.Row>
                <Form.Group>
                  <Form.Label for_="apiUrl">
                    {ReasonReact.string("API URL")}
                  </Form.Label>
                  <Form.TextInput
                    id="apiUrl"
                    placeholder="http://localhost:5000/graphql"
                    value=Config.(state.apiUrl)
                    onChange={url => send(SetUrl(url))}
                  />
                </Form.Group>
              </Layout.Row>
              <Layout.Row>
                <Form.Group>
                  <Form.Label for_="authHeader">
                    {ReasonReact.string("Authorization header")}
                  </Form.Label>
                  <Form.TextInput
                    id="authHeader"
                    placeholder="Bearer ..."
                    value=Config.(state.authHeader)
                    onChange={authHeader => send(SetAuthHeader(authHeader))}
                  />
                </Form.Group>
              </Layout.Row>
              <Form.SubmitButton>
                {ReasonReact.string("Save")}
              </Form.SubmitButton>
            </Form>
          </Layout.Card>
        </Layout.Row>
      </div>
    </div>,
};