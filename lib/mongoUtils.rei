/**
   This module contains some utility tools or helpers
*/;

let print_buffer: string => unit;

let cur_timestamp: unit => int32;

let encode_int32: (Buffer.t, int32) => unit;

let encode_int64: (Buffer.t, int64) => unit;

let encode_cstring: (Buffer.t, string) => unit;

let decode_int32: (string, int) => (int32, int);

let decode_int64: (string, int) => (int64, int);

let decode_cstring: (string, int) => (string, int);
