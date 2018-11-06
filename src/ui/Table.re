let component = ReasonReact.statelessComponent("Table");

let make = children => {
  ...component,
  render: _self =>
    <table
      className="table table-responsive-sm table-bordered table-striped table-sm">
      ...children
    </table>,
};

module Head = {
  let component = ReasonReact.statelessComponent("Table.Head");

  let make = children => {
    ...component,
    render: _self => <thead> <tr> ...children </tr> </thead>,
  };
};

module HeadColumn = {
  let component = ReasonReact.statelessComponent("Table.HeadColumn");

  let make = children => {
    ...component,
    render: _self => <th> ...children </th>,
  };
};

module Rows = {
  let component = ReasonReact.statelessComponent("Table.Rows");

  let make = children => {
    ...component,
    render: _self => <tbody> ...children </tbody>,
  };
};

module Row = {
  let component = ReasonReact.statelessComponent("Table.Row");

  let make = children => {
    ...component,
    render: _self => <tr> ...children </tr>,
  };
};

module Cell = {
  let component = ReasonReact.statelessComponent("Table.Cell");

  let make = children => {
    ...component,
    render: _self => <td> ...children </td>,
  };
};