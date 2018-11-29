open Belt;

/* Render a list of ReasonReact.reactElement */
let list = list => list->List.toArray->ReasonReact.array;