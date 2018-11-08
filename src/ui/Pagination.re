open Belt;

let component = ReasonReact.statelessComponent("Pagination");

/* shame to Reason stdlib for not having this :( */
let rec range = (start: int, end_: int) =>
  if (start > end_) {
    [];
  } else {
    [start, ...range(start + 1, end_)];
  };

module Item = {
  let component = ReasonReact.statelessComponent("Pagination.Item");

  let make = (~onClick=?, ~isActive=?, ~isDisabled=?, children) => {
    ...component,
    render: _self => {
      let className =
        "page-item"
        ++ (isActive == Some(true) ? " active" : "")
        ++ (isDisabled == Some(true) ? " disabled" : "");
      <li className> <a className="page-link" ?onClick> ...children </a> </li>;
    },
  };
};

let make = (~onChange, ~totalCount, ~itemsPerPage, ~currentPage, _children) => {
  ...component,
  render: _self => {
    let pagesCount = totalCount / itemsPerPage + 1;
    <nav>
      <ul className="pagination">
        <Item
          isDisabled={currentPage <= 1}
          onClick={_ => onChange(currentPage - 1)}>
          {ReasonReact.string("Prev")}
        </Item>
        {
          range(1, pagesCount)
          ->List.map(pageNumber =>
              <Item
                key={string_of_int(pageNumber)}
                isActive={currentPage === pageNumber}
                onClick={_ => onChange(pageNumber)}>
                {ReasonReact.string(string_of_int(pageNumber))}
              </Item>
            )
          ->List.toArray
          ->ReasonReact.array
        }
        <Item
          isDisabled={currentPage >= pagesCount}
          onClick={_ => onChange(currentPage + 1)}>
          {ReasonReact.string("Next")}
        </Item>
      </ul>
    </nav>;
  },
};