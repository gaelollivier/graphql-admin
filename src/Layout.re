module Header = {
  let component = ReasonReact.statelessComponent("Layout.Header");

  let make = children => {
    ...component,
    render: _self =>
      <header className="app-header navbar">
        <a className="navbar-brand" href="#"> ...children </a>
      </header>,
  };
};

module Body = {
  let component = ReasonReact.statelessComponent("Layout.Body");

  let make = children => {
    ...component,
    render: _self => <div className="app-body"> ...children </div>,
  };
};

module Sidebar = {
  let component = ReasonReact.statelessComponent("Layout.Sidebar");

  let make = children => {
    ...component,
    render: _self =>
      <div className="sidebar">
        <nav className="sidebar-nav">
          <ul className="nav"> ...children </ul>
        </nav>
      </div>,
  };
};

module SidebarItem = {
  let component = ReasonReact.statelessComponent("Layout.SidebarItem");

  let make = children => {
    ...component,
    render: _self =>
      <li className="nav-item">
        <a className="nav-link" href="colors.html"> ...children </a>
      </li>,
  };
};

module Breadcrumb = {
  let component = ReasonReact.statelessComponent("Layout.Breadcrumb");

  let make = _children => {
    ...component,
    render: _self =>
      <ol className="breadcrumb">
        <li className="breadcrumb-item"> {ReasonReact.string("Home")} </li>
        <li className="breadcrumb-item">
          <a href="#"> {ReasonReact.string("Admin")} </a>
        </li>
        <li className="breadcrumb-item active">
          {ReasonReact.string("Dashboard")}
        </li>
      </ol>,
  };
};

module Content = {
  let component = ReasonReact.statelessComponent("Layout.Content");

  let make = children => {
    ...component,
    render: _self =>
      <main className="main">
        <Breadcrumb />
        <div className="container-fluid">
          <div className="animated fadeIn"> ...children </div>
        </div>
      </main>,
  };
};

module Row = {
  let component = ReasonReact.statelessComponent("Layout.Row");

  let make = children => {
    ...component,
    render: _self =>
      <div className="row">
        <div className="col-lg-12"> ...children </div>
      </div>,
  };
};

module Card = {
  let component = ReasonReact.statelessComponent("Layout.Card");

  let make = (~title: string, children) => {
    ...component,
    render: _self =>
      <div className="card">
        <div className="card-header">
          <i className="fa fa-align-justify" />
          {ReasonReact.string(title)}
        </div>
        <div> ...children </div>
      </div>,
  };
};