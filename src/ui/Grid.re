module Row = {
  let component = ReasonReact.statelessComponent("Grid.Row");

  let make = children => {
    ...component,
    render: _self => <div className="row"> ...children </div>,
  };
};

module Cell = {
  let component = ReasonReact.statelessComponent("Grid.Cell");

  let make = (~size, children) => {
    ...component,
    render: _self =>
      <div className={"col-sm-" ++ string_of_int(size)}> ...children </div>,
  };
};