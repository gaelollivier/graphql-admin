open Belt;

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self =>
    <FetchQuery query=Schema.introspectionQuery>
      ...{
           introspectionRes => {
             let schema = Schema.decodeIntrospectionQuery(introspectionRes);
             /* schema.queryFields->List.forEach(field => Js.log(field.name)); */
             let licenseType = schema.types->Map.String.getExn("License");
             <Layout>
               <FetchQuery
                 query="{ licenses { id description implementation nickname spdxId url } }">
                 ...{
                      tableRes =>
                        <ResultTable json=tableRes rowType=licenseType />
                    }
               </FetchQuery>
             </Layout>;
           }
         }
    </FetchQuery>,
};