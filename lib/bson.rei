/**
    This module includes a Bson document data structure, together with its encoding (to bytes) and decoding (from bytes).

    The logic of {b usage} is like this
    - Create an empty Bson document
    - Create the elements you want
    - Add elements to the document with names
    - Or remove elements from the document via the names
    - Get elements from the document via the names
    - After obtaining an element, get the raw value from the element

    The functions inside this module seem to be many, however, most of them are just for creating elements. These functions are to {e hide the implementation details of the type elements}. Also, in this way, the Bson document can be used more safely.

    Please refer to the {{: http://bsonspec.org/#/specification } Official Bson specification } for more information.

    {e Version 0.89.1}
*/;

/** Raised when an unkown bson type is met while encoding the bson doc */

exception WrongBsonType;

/** Raised only when trying to decode the bytes to string. */

exception Wrong_string;

/** Raised when bad things happen while decoding the bytes to bson doc */

exception Malformed_bson;

/** The type for the Bson document. This is the main data structure */

type t;

/** The type for representing the special fields in Bson */

type special =
  | NULL
  | MINKEY
  | MAXKEY;

/** The type for the fields for the Bson document */

type element;

/** {6 Basic operations on Bson document} */;

/** The empty Bson document */

let empty: t;

/** Check whether this Bson document empty or not */

let isEmpty: t => bool;

/** Add an element to a Bson document */

let addElement: (string, element, t) => t;

/** Create a Bson document from a list of [(key, element), ...] **/
let fromElements: list((string, element)) => t;

/** Get an element from a Bson document via its name */

let getElement: (string, t) => element;

/** Check whether this Bson document has a specific element */

let hasElement: (string, t) => bool;

/** Remove an element from a Bson document */

let removeElement: (string, t) => t;

/** Encode a Bson document to bytes (using type string as a carrier) */

let encode: t => string;

/** Decode bytes (assuming type string as a carrier) to a Bson document */

let decode: string => t;

/** {6 Creating elements} */;

let createDouble: float => element;
let createString: string => element;
let createDocElement: t => element;
let createList: list(element) => element;
let createDocElementList: list(t) => element;
let createObjectId: ObjectId.t => element;
let createBoolean: bool => element;
let createUtc: int64 => element;
let createNull: unit => element;
let createRegex: (string, string) => element;
let createInt32: int32 => element;
let createInt64: int64 => element;
let create_user_binary: string => element;

/** {6 Getting raw values from elements} */;

let getDouble: element => float;
let getString: element => string;
let getDocElement: element => t;
let getList: element => list(element);
let getObjectId: element => ObjectId.t;
let getBoolean: element => bool;
let getUtc: element => int64;
let getNull: element => special;
let getRegex: element => (string, string);
let getInt32: element => int32;
let getInt64: element => int64;
let getTimestamp: element => int64;

let get_generic_binary: element => string;
let get_function_binary: element => string;
let get_uuid_binary: element => string;
let get_md5_binary: element => string;
let get_user_binary: element => string;
let get_jscode: element => string;
let get_jscode_w_s: element => (string, t);
let get_minkey: element => special;
let get_maxkey: element => special;

let allElements: t => list((string, element));

/** {6 Experimental. Convert a Bson document to Json.} */;

let toSimpleJson: t => string;