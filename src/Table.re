let component = ReasonReact.statelessComponent("Table");

let make = _children => {
  ...component,
  render: _self =>
    <div className="card-body">
      <table
        className="table table-responsive-sm table-bordered table-striped table-sm">
        <thead>
          <tr>
            <th> {ReasonReact.string("Username")} </th>
            <th> {ReasonReact.string("Date registered")} </th>
            <th> {ReasonReact.string("Role")} </th>
            <th> {ReasonReact.string("Status")} </th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td> {ReasonReact.string("Vishnu Serghei")} </td>
            <td> {ReasonReact.string("2012/01/01")} </td>
            <td> {ReasonReact.string("Member")} </td>
            <td>
              <span className="badge badge-success">
                {ReasonReact.string("Active")}
              </span>
            </td>
          </tr>
        </tbody>
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