/** Raised when trying to create an ObjectID from an invalid string value */

exception InvalidObjectId;

type t;

/* Creates an ObjectID from a binary string containing the raw ObjectID bytes */
let fromBinaryString: string => t;

/* Returns a binary string containing the raw ObjectID bytes */
let toBinaryString: t => string;

/* Returns a string representation of ObjectID bytes as hexadecimal */
let toString: t => string;

/* Generate a new (probably) unique ObjectId */
let generate: unit => t;