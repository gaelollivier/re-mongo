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
let get_file_descr: t => Unix.file_descr;

/** {6 Lifecycle of a MongoAdmin} */;

/** create a MongoAdmin. e.g. create ip port. May raise MongoAdmin_failed exception.*/

let create: (string, int) => t;

/** create a MongoAdmin connecting to 127.0.0.1, port 27017. May raise MongoAdmin_failed exception.*/

let create_local_default: unit => t;

/** destroy a MongoAdmin. Please use this to destroy a MongoAdmin once it finishes its purpose, in order to release system resources. May raise MongoAdmin_failed exception.*/

let destroy: t => unit;

/** {6 Commands via a MongoAdmin, may raise MongoAdmin_failed exception.} */;

let listDatabases: t => MongoReply.t;
let buildInfo: t => MongoReply.t;
let collStats: t => MongoReply.t;
let connPoolStats: t => MongoReply.t;
let cursorInfo: t => MongoReply.t;
let getCmdLineOpts: t => MongoReply.t;
let hostInfo: t => MongoReply.t;
let listCommands: t => MongoReply.t;
let serverStatus: t => MongoReply.t;
