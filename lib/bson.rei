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

exception Wrong_bson_type;

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

let is_empty: t => bool;

/** Add an element to a Bson document */

let add_element: (string, element, t) => t;

/** Get an element from a Bson document via its name */

let get_element: (string, t) => element;

/** Check whether this Bson document has a specific element */

let has_element: (string, t) => bool;

/** Remove an element from a Bson document */

let remove_element: (string, t) => t;

/** Encode a Bson document to bytes (using type string as a carrier) */

let encode: t => string;

/** Decode bytes (assuming type string as a carrier) to a Bson document */

let decode: string => t;

/** {6 Creating elements} */;

let create_double: float => element;
let create_string: string => element;
let create_doc_element: t => element;
let create_list: list(element) => element;
let create_doc_element_list: list(t) => element;
let create_user_binary: string => element;
let create_objectId: ObjectId.t => element;
let create_boolean: bool => element;
let create_utc: int64 => element;
let create_null: unit => element;
let create_regex: (string, string) => element;
let create_jscode: string => element;
let create_jscode_w_s: (string, t) => element;
let create_int32: int32 => element;
let create_int64: int64 => element;
let create_minkey: unit => element;
let create_maxkey: unit => element;

/** {6 Getting raw values from elements} */;

let get_double: element => float;
let get_string: element => string;
let get_doc_element: element => t;
let get_list: element => list(element);
let get_generic_binary: element => string;
let get_function_binary: element => string;
let get_uuid_binary: element => string;
let get_md5_binary: element => string;
let get_user_binary: element => string;
let get_objectId: element => ObjectId.t;
let get_boolean: element => bool;
let get_utc: element => int64;
let get_null: element => special;
let get_regex: element => (string, string);
let get_jscode: element => string;
let get_jscode_w_s: element => (string, t);
let get_int32: element => int32;
let get_int64: element => int64;
let get_timestamp: element => int64;
let get_minkey: element => special;
let get_maxkey: element => special;

let all_elements: t => list((string, element));

/** {6 Experimental. Convert a Bson document to Json.} */;

let to_simple_json: t => string;

/*val create_generic_binary : string -> element;;
  val create_function_binary : string -> element;;
  val create_uuid_binary : string -> element;;
  val create_md5_binary : string -> element;;
  val create_timestamp : int64 -> element;;*/