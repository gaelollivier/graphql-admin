let component = ReasonReact.statelessComponent("Form");

let make = (~onSubmit, children) => {
  ...component,
  render: _self => <form onSubmit={_ => onSubmit()}> ...children </form>,
};

module Group = {
  let component = ReasonReact.statelessComponent("Form.Group");

  let make = children => {
    ...component,
    render: _self => <div className="form-group"> ...children </div>,
  };
};

module Label = {
  let component = ReasonReact.statelessComponent("Form.Label");

  let make = (~for_, children) => {
    ...component,
    render: _self => <label htmlFor=for_> ...children </label>,
  };
};

module TextInput = {
  let component = ReasonReact.statelessComponent("Form.TextInput");

  let make = (~id, ~placeholder, ~value, ~onChange, _children) => {
    ...component,
    render: _self =>
      <input
        className="form-control"
        id
        type_="text"
        placeholder
        value
        onChange={event => onChange(event->ReactEvent.Form.target##value)}
      />,
  };
};

module SubmitButton = {
  let component = ReasonReact.statelessComponent("SubmitButton");

  let make = children => {
    ...component,
    render: _self =>
      <button className="btn btn-sm btn-primary" type_="submit">
        ...children
      </button>,
  };
};