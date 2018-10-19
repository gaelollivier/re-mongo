open ReMongo;

let doc = Bson.empty |> Bson.add_element("name", Bson.create_string("test"));

print_endline(doc |> Bson.to_simple_json);