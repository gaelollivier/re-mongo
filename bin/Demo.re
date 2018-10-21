open ReMongo;

let doc =
  Bson.empty
  |> Bson.add_element("a", Bson.create_objectId(ObjectId.generate()))
  |> Bson.add_element("b", Bson.create_objectId(ObjectId.generate()))
  |> Bson.add_element("c", Bson.create_objectId(ObjectId.generate()));

print_endline(doc |> Bson.to_simple_json);

let start = () => {
  let%lwt db = Mongo_lwt.create_local_default("db", "users");
  let%lwt res = Mongo_lwt.find(db);
  let _ =
    res
    |> MongoReply.get_document_list
    |> List.map(Bson.to_simple_json)
    |> List.map(print_endline);
  Lwt.return(0);
};

Lwt_main.run(start());