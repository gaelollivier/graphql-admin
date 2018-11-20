/* Link component that triggers a route change with ReasonReact router */

let component = ReasonReact.statelessComponent("Link");

let make = (~className=?, ~url, children) => {
  ...component,
  render: _self =>
    <a
      ?className
      href=url
      onClick={
        event => {
          event->ReactEvent.Mouse.preventDefault;
          ReasonReact.Router.push(url);
        }
      }>
      ...children
    </a>,
};