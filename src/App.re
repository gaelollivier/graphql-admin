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
             let fieldName = "marketplaceCategories";
             let rowType =
               schema.types->Map.String.getExn("MarketplaceCategory");
             let licenseFields =
               rowType.fields
               ->Option.getExn
               ->List.keep(field => field.type_->Schema.isDisplayable)
               ->List.map(field => field.name)
               |> String.concat(" ");
             <Layout>
               <FetchQuery
                 query={"{ " ++ fieldName ++ " { " ++ licenseFields ++ " } }"}>
                 ...{
                      tableRes => {
                        let json =
                          tableRes
                          |> Json.Decode.at(["data", fieldName], x => x);
                        <ResultTable json rowType />;
                      }
                    }
               </FetchQuery>
             </Layout>;
           }
         }
    </FetchQuery>,
};