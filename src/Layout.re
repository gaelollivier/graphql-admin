let component = ReasonReact.statelessComponent("Layout");

let make = children => {
  ...component,
  render: _self =>
    <>
      <header className="app-header navbar">
        <a className="navbar-brand" href="#">
          {ReasonReact.string("GraphQL Admin")}
        </a>
      </header>
      <div className="app-body">
        <div className="sidebar">
          <nav className="sidebar-nav">
            <ul className="nav">
              <li className="nav-item">
                <a className="nav-link" href="colors.html">
                  <i className="nav-icon icon-speedometer" />
                  {ReasonReact.string(" Index")}
                </a>
              </li>
              <li className="nav-title">
                {ReasonReact.string(" Category")}
              </li>
              <li className="nav-item">
                <a className="nav-link" href="colors.html">
                  <i className="nav-icon icon-drop" />
                  {ReasonReact.string(" Item")}
                </a>
              </li>
            </ul>
          </nav>
        </div>
        <main className="main">
          /* Breadcrumb */

            <ol className="breadcrumb">
              <li className="breadcrumb-item">
                {ReasonReact.string("Home")}
              </li>
              <li className="breadcrumb-item">
                <a href="#"> {ReasonReact.string("Admin")} </a>
              </li>
              <li className="breadcrumb-item active">
                {ReasonReact.string("Dashboard")}
              </li>
            </ol>
            <div className="container-fluid">
              <div className="animated fadeIn">
                <div className="row">
                  <div className="col-lg-12">
                    <div className="card">
                      <div className="card-header">
                        <i className="fa fa-align-justify" />
                        {ReasonReact.string(" Table")}
                      </div>
                      <div> ...children </div>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </main>
      </div>
    </>,
};