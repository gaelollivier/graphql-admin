let component = ReasonReact.statelessComponent("Table");

let make = children => {
  ...component,
  render: _self =>
    <div className="card-body">
      <table
        className="table table-responsive-sm table-bordered table-striped table-sm">
        ...children
      </table>
      <nav>
        <ul className="pagination">
          <li className="page-item">
            <a className="page-link" href="#">
              {ReasonReact.string("Prev")}
            </a>
          </li>
          <li className="page-item active">
            <a className="page-link" href="#"> {ReasonReact.string("1")} </a>
          </li>
          <li className="page-item">
            <a className="page-link" href="#"> {ReasonReact.string("2")} </a>
          </li>
          <li className="page-item">
            <a className="page-link" href="#">
              {ReasonReact.string("Next")}
            </a>
          </li>
        </ul>
      </nav>
    </div>,
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