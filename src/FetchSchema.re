let component = ReasonReact.statelessComponent("FetchSchema");

let make = children => {
  ...component,
  render: _self =>
    <FetchQuery query=Schema.introspectionQuery>
      ...{
           introspectionRes => {
             let schema = Schema.decodeIntrospectionQuery(introspectionRes);
             <Schema.Context.Provider value=schema>
               ...children
             </Schema.Context.Provider>;
           }
         }
    </FetchQuery>,
};