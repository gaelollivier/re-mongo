/**
   {b This is a major client-faced module, for the high level usage.}

   This module includes a series of APIs that client can use directly to obtain some admin level of information about a MongoDB, e.g., the list of databases inside, etc.
   Currently all commands are read-only ones, which means they are just retrieving infomation from MongoDB, not setting any configurations.

   Please refer to {{: http://docs.mongodb.org/manual/reference/command/ } MongoDB Commands } for more information
*/;

/** the exception will be raised if anything is wrong, with a string message */

exception MongoAdmin_failed(string);

/** the type for MongoAdmin */

type t;

/** {6 Essential info of a MongoAdmin} */;

let get_db_name: t => string;
let get_collection_name: t => string;
let get_ip: t => string;
let get_port: t => int;
let get_channel_pool:
  t => Lwt_pool.t((Lwt_io.input_channel, Lwt_io.output_channel));
/* val get_channels: t -> (Lwt_io.input_channel * Lwt_io.output_channel);; */
/* val get_output_channel: t -> Lwt_io.output_channel;; */
/* val get_input_channel: t -> Lwt_io.input_channel;; */

/** {6 Lifecycle of a MongoAdmin} */;

/** create a MongoAdmin. e.g. create ip port. May raise MongoAdmin_failed exception.*/

let create: (~max_connection: int=?, string, int) => Lwt.t(t);

/** create a MongoAdmin connecting to 127.0.0.1, port 27017. May raise MongoAdmin_failed exception.*/

let createLocalDefault: unit => Lwt.t(t);

/** destroy a MongoAdmin. Please use this to destroy a MongoAdmin once it finishes its purpose, in order to release system resources. May raise MongoAdmin_failed exception.*/

let destroy: t => Lwt.t(unit);

/** {6 Commands via a MongoAdmin, may raise MongoAdmin_failed exception.} */;

let listDatabases: t => Lwt.t(MongoReply.t);
let buildInfo: t => Lwt.t(MongoReply.t);
let collStats: t => Lwt.t(MongoReply.t);
let connPoolStats: t => Lwt.t(MongoReply.t);
let cursorInfo: t => Lwt.t(MongoReply.t);
let getCmdLineOpts: t => Lwt.t(MongoReply.t);
let hostInfo: t => Lwt.t(MongoReply.t);
let listCommands: t => Lwt.t(MongoReply.t);
let serverStatus: t => Lwt.t(MongoReply.t);