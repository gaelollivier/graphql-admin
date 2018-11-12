type t = {
  name: string,
  queryField: string,
  columns: list(string),
  /* TODO: remove from here and pass as context/props */
  schema: Schema.t,
};