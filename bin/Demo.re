open ReMongo;

let doc =
  Bson.fromElements([
    ("_id", Bson.createObjectId(ObjectId.generate())),
    ("email", Bson.createString("hello@world.com")),
  ]);

print_endline(doc |> Bson.toSimpleJson);

let start = () => {
  let%lwt db = Mongo_lwt.createLocalDefault("db", "users");
  let%lwt res = Mongo_lwt.find(db);
  let _ =
    res
    |> MongoReply.getDocumentList
    |> List.map(Bson.toSimpleJson)
    |> List.map(print_endline);
  Lwt.return(0);
};

Lwt_main.run(start());