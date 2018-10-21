/**
    This module defines reply that MongoDB sends back for a query.
*/;

/** type of reply sent from MongoDB*/

type t;

/** decode a string received from MongoDB to a MongoReply */

let decode_reply: string => t;

/** {6 Values from a MongoReply} */;

/** get the MongoHeader out of a MongoReply */

let get_header: t => MongoHeader.t;

/** get the response_flags field out of a MongoReply */

let get_response_flags: t => int32;

/** get the cursor_id out of a MongoReply */

let get_cursor: t => int64;

/** get the starting_from field out of a MongoReply */

let get_starting_from: t => int32;

/** get the number of items returned out of a MongoReply */

let get_num_returned: t => int32;

/** get the document list returned out of a MongoReply */

let getDocumentList: t => list(Bson.t);

/** {6 Operations} */;

/** convert a MongoReply to a human readable string */

let to_string: t => string;