/* Converted from Ocaml: https://github.com/MassD/bson/blob/master/src/bson.ml */

exception Invalid_objectId;
exception Wrong_bson_type;
exception Wrong_string;
exception Malformed_bson;

type document = list((string, element))
and t = document
and special =
  | NULL
  | MINKEY
  | MAXKEY
and element =
  | Double(float)
  | String(string)
  | Document(document)
  | Array(list(element))
  | Binary(binary)
  | ObjectId(string) /* only 12 bytes */
  | Boolean(bool)
  | UTC(int64)
  | Null(special)
  | Regex((string, string))
  | JSCode(string)
  | JSCodeWS((string, document))
  | Int32(int32)
  | Int64(int64)
  | Timestamp(int64)
  | MinKey(special)
  | MaxKey(special)
and binary =
  | Generic(string)
  | Function(string)
  | UUID(string)
  | MD5(string)
  | UserDefined(string);

let empty = [];

let is_empty =
  fun
  | [] => true
  | _ => false;

let has_element = List.mem_assoc;

/*
   The remove  operations.
 */
let remove_element = List.remove_assoc;

/*
   for constructing a document
   1. we make a empty document
   2. we create element as we want
   3. we add the element to the document, with a element name
 */
let add_element = (ename, element, doc) => {
  /* Emulating StringMap add operation */
  let doc =
    if (has_element(ename, doc)) {
      remove_element(ename, doc);
    } else {
      doc;
    };

  [(ename, element), ...doc];
};

/*
   for using a document
   1. we get an element from document, if existing
   2. we get the value of the element
 */
let get_element = List.assoc;

let create_double = v => Double(v);
let create_string = v => String(v);
let create_doc_element = v => Document(v);
let create_list = l => Array(l);
let create_doc_element_list = l =>
  create_list(List.map(create_doc_element, l));
/* let create_generic_binary v = Binary (Generic v);;
   let create_function_binary v = Binary (Function v);;
   let create_uuid_binary v = Binary (UUID v);;
   let create_md5_binary v = Binary (MD5 v);; */
let create_user_binary = v => Binary(UserDefined(v));
/* let is_valid_objectId objectId = if String.length objectId = 12 || String.length objectId = 24 then true else false;; */
let hex_to_string = s => {
  let n = String.length(s);
  let buf = Buffer.create(12);
  let rec convert = i =>
    if (i > n - 1) {
      Buffer.contents(buf);
    } else {
      Buffer.add_char(
        buf,
        char_of_int(int_of_string("0x" ++ String.sub(s, i, 2))),
      );
      convert(i + 2);
    };

  convert(0);
};
let create_objectId = v =>
  if (String.length(v) == 12) {
    ObjectId(v);
  } else if (String.length(v) == 24) {
    try (ObjectId(hex_to_string(v))) {
    | Failure("int_of_string") => raise(Invalid_objectId)
    };
  } else {
    raise(Invalid_objectId);
  };
let create_boolean = v => Boolean(v);
let create_utc = v => UTC(v);
let create_null = () => Null(NULL);
let create_regex = (s1, s2) => [@implicit_arity] Regex(s1, s2);
let create_jscode = v => JSCode(v);
let create_jscode_w_s = (s, doc) => [@implicit_arity] JSCodeWS(s, doc);
let create_int32 = v => Int32(v);
let create_int64 = v => Int64(v);
/* let create_timestamp v = Timestamp v;; */
let create_minkey = () => MinKey(MINKEY);
let create_maxkey = () => MaxKey(MAXKEY);

let get_double =
  fun
  | Double(v) => v
  | _ => raise(Wrong_bson_type);
let get_string =
  fun
  | String(v) => v
  | _ => raise(Wrong_bson_type);
let get_doc_element =
  fun
  | Document(v) => v
  | _ => raise(Wrong_bson_type);
let get_list =
  fun
  | Array(v) => v
  | _ => raise(Wrong_bson_type);
let get_generic_binary =
  fun
  | Binary(Generic(v)) => v
  | _ => raise(Wrong_bson_type);
let get_function_binary =
  fun
  | Binary(Function(v)) => v
  | _ => raise(Wrong_bson_type);
let get_uuid_binary =
  fun
  | Binary(UUID(v)) => v
  | _ => raise(Wrong_bson_type);
let get_md5_binary =
  fun
  | Binary(MD5(v)) => v
  | _ => raise(Wrong_bson_type);
let get_user_binary =
  fun
  | Binary(UserDefined(v)) => v
  | _ => raise(Wrong_bson_type);
let get_objectId =
  fun
  | ObjectId(v) => v
  | _ => raise(Wrong_bson_type);
let get_boolean =
  fun
  | Boolean(v) => v
  | _ => raise(Wrong_bson_type);
let get_utc =
  fun
  | UTC(v) => v
  | _ => raise(Wrong_bson_type);
let get_null =
  fun
  | Null(NULL) => NULL
  | _ => raise(Wrong_bson_type);
let get_regex =
  fun
  | Regex(v) => v
  | _ => raise(Wrong_bson_type);
let get_jscode =
  fun
  | JSCode(v) => v
  | _ => raise(Wrong_bson_type);
let get_jscode_w_s =
  fun
  | JSCodeWS(v) => v
  | _ => raise(Wrong_bson_type);
let get_int32 =
  fun
  | Int32(v) => v
  | _ => raise(Wrong_bson_type);
let get_int64 =
  fun
  | Int64(v) => v
  | _ => raise(Wrong_bson_type);
let get_timestamp =
  fun
  | Timestamp(v) => v
  | _ => raise(Wrong_bson_type);
let get_minkey =
  fun
  | MinKey(MINKEY) => MINKEY
  | _ => raise(Wrong_bson_type);
let get_maxkey =
  fun
  | MaxKey(MAXKEY) => MAXKEY
  | _ => raise(Wrong_bson_type);

let all_elements = d => d;

/*
   encode int64, int32 and float.
   note that encoding float is the same as int64, just need to transfer all the bits into an int64.

   The logic is that (e.g., for int32):
   1) we get an int32
   2) we shift right 1 byte one by one
   3) After each shift, we logic and 0000 0000 ... 0000 1111 1111 (255l) with the shifted int32 to get the lower 1 byte
   4) we convert the int32 to int, so Char.chr can pick it up and convert it to char (byte)
   5) we put the byte to the buffer (starting from index of 0, since it is little-endian format)
 */

let encode_int64 = (buf, v) =>
  for (i in 0 to 7) {
    let b = Int64.logand(255L, Int64.shift_right(v, i * 8));
    Buffer.add_char(buf, Char.chr(Int64.to_int(b)));
  };

let encode_float = (buf, v) => encode_int64(buf, Int64.bits_of_float(v));

let encode_int32 = (buf, v) =>
  for (i in 0 to 3) {
    let b = Int32.logand(255l, Int32.shift_right(v, i * 8));
    Buffer.add_char(buf, Char.chr(Int32.to_int(b)));
  };

let encode_ename = (buf, c, ename) => {
  Buffer.add_char(buf, c);
  Buffer.add_string(buf, ename);
  Buffer.add_char(buf, '\000');
};

let encode_string = (buf, s) => {
  let len = String.length(s);
  if (len > 0 && s.[len - 1] == '\000') {
    raise(Wrong_string);
  } else {
    encode_int32(buf, Int32.of_int(len + 1));
    Buffer.add_string(buf, s);
    Buffer.add_char(buf, '\000');
  };
};

let encode_objectId = (buf, s) =>
  if (String.length(s) != 12) {
    raise(Invalid_objectId);
  } else {
    Buffer.add_string(buf, s);
  };

let encode_binary = (buf, c, b) => {
  encode_int32(buf, Int32.of_int(String.length(b)));
  Buffer.add_char(buf, c);
  Buffer.add_string(buf, b);
};

let encode_cstring = (buf, cs) => {
  Buffer.add_string(buf, cs);
  Buffer.add_char(buf, '\000');
};

let list_to_doc = l => {
  /* we need to transform the list to a doc with key as incrementing from '0' */
  let rec to_doc = (i, acc) =>
    fun
    | [] => acc
    | [hd, ...tl] =>
      to_doc(i + 1, add_element(string_of_int(i), hd, acc), tl);

  to_doc(0, empty, l);
};

let encode = doc => {
  let all_buf = Buffer.create(64);
  let rec encode_element = (buf, ename, element) =>
    switch (element) {
    | Double(v) =>
      encode_ename(buf, '\001', ename);
      encode_float(buf, v);
    | String(v) =>
      encode_ename(buf, '\002', ename);
      encode_string(buf, v);
    | Document(v) =>
      encode_ename(buf, '\003', ename);
      encode_doc(buf, v);
    | Array(v) =>
      encode_ename(buf, '\004', ename);
      encode_doc(buf, list_to_doc(v));
    | Binary(v) =>
      encode_ename(buf, '\005', ename);
      switch (v) {
      | Generic(v) => encode_binary(buf, '\000', v)
      | Function(v) => encode_binary(buf, '\001', v)
      | UUID(v) => encode_binary(buf, '\004', v)
      | MD5(v) => encode_binary(buf, '\005', v)
      | UserDefined(v) => encode_binary(buf, '\128', v)
      };
    | ObjectId(v) =>
      encode_ename(buf, '\007', ename);
      encode_objectId(buf, v);
    | Boolean(v) =>
      encode_ename(buf, '\b', ename);
      Buffer.add_char(buf, if (v) {'\001'} else {'\000'});
    | UTC(v) =>
      encode_ename(buf, '\t', ename);
      encode_int64(buf, v);
    | Null(NULL) => encode_ename(buf, '\n', ename)
    | [@implicit_arity] Regex(v1, v2) =>
      encode_ename(buf, '\011', ename);
      encode_cstring(buf, v1);
      encode_cstring(buf, v2);
    | JSCode(v) =>
      encode_ename(buf, '\r', ename);
      encode_string(buf, v);
    | [@implicit_arity] JSCodeWS(v, d) =>
      encode_ename(buf, '\015', ename);
      let tmp_str_buf = Buffer.create(16)
      and tmp_doc_buf = Buffer.create(16);
      encode_string(tmp_str_buf, v);
      encode_doc(tmp_doc_buf, d);
      encode_int32(
        buf,
        Int32.of_int(
          4 + Buffer.length(tmp_str_buf) + Buffer.length(tmp_doc_buf),
        ),
      );
      Buffer.add_buffer(buf, tmp_str_buf);
      Buffer.add_buffer(buf, tmp_doc_buf);
    | Int32(v) =>
      encode_ename(buf, '\016', ename);
      encode_int32(buf, v);
    | Timestamp(v) =>
      encode_ename(buf, '\017', ename);
      encode_int64(buf, v);
    | Int64(v) =>
      encode_ename(buf, '\018', ename);
      encode_int64(buf, v);
    | MinKey(MINKEY) => encode_ename(buf, '\255', ename)
    | MaxKey(MAXKEY) => encode_ename(buf, '\127', ename)
    | _ => raise(Malformed_bson)
    }
  and encode_doc = (buf, doc) => {
    let process_element = (buf, (ename, element)) => {
      encode_element(buf, ename, element);
      buf;
    };
    let e_buf = List.fold_left(process_element, Buffer.create(64), doc);
    encode_int32(buf, Int32.of_int(5 + Buffer.length(e_buf)));
    Buffer.add_buffer(buf, e_buf);
    Buffer.add_char(buf, '\000');
  };

  encode_doc(all_buf, doc);
  Buffer.contents(all_buf);
};

let decode_int64 = (str, cur) => {
  let rec decode = (i, acc) =>
    if (i < cur) {
      acc;
    } else {
      let high_byte = Char.code(str.[i]);
      let high_int64 = Int64.of_int(high_byte);
      let shift_acc = Int64.shift_left(acc, 8);
      let new_acc = Int64.logor(high_int64, shift_acc);
      decode(i - 1, new_acc);
    };
  (decode(cur + 7, 0L), cur + 8);
};

let decode_float = (str, cur) => {
  let (i, new_cur) = decode_int64(str, cur);
  (Int64.float_of_bits(i), new_cur);
};

let decode_int32 = (str, cur) => {
  let rec decode = (i, acc) =>
    if (i < cur) {
      acc;
    } else {
      let high_byte = Char.code(str.[i]);
      /*print_int high_byte;print_endline "";*/
      let high_int32 = Int32.of_int(high_byte);
      let shift_acc = Int32.shift_left(acc, 8);
      let new_acc = Int32.logor(high_int32, shift_acc);
      decode(i - 1, new_acc);
    };
  (decode(cur + 3, 0l), cur + 4);
};

let next_x00 = (str, cur) => String.index_from(str, cur, '\000');

let decode_ename = (str, cur) => {
  let x00 = next_x00(str, cur);
  if (x00 == (-1)) {
    raise(Malformed_bson);
  } else {
    (String.sub(str, cur, x00 - cur), x00 + 1);
  };
};

let decode_cstring = decode_ename;

let decode_len = (str, cur) => {
  let (len32, next_cur) = decode_int32(str, cur);
  (Int32.to_int(len32), next_cur);
};

let decode_double = (str, cur) => {
  let (f, new_cur) = decode_float(str, cur);
  (Double(f), new_cur);
};

let decode_string = (str, cur) => {
  let (len, next_cur) = decode_len(str, cur);
  /*print_string "cur=";print_int cur;print_string ";";
    print_string "len=";print_int len;
    print_endline "";*/
  /*let x00 = next_x00 str next_cur in
    Printf.printf "len=%d, next_cur=%d, x00=%d, s[x00]=%c\n" len next_cur x00 str.[x00-1];
    print_endline (String.sub str next_cur (len-1));*/
  /*if len - 1 <> x00-next_cur then raise Wrong_string
    else (String.sub str next_cur (len-1), x00+1);;*/
  (String.sub(str, next_cur, len - 1), next_cur + len);
};

let doc_to_list = doc =>
  /* we need to transform a doc with key as incrementing from '0' to a list */
  List.map(((_, v)) => v, doc);

let decode_binary = (str, cur) => {
  let (len, next_cur) = decode_len(str, cur);
  let c = str.[next_cur];
  let b = String.sub(str, next_cur + 1, len);
  let new_cur = next_cur + 1 + len;
  switch (c) {
  | '\000' => (Binary(Generic(b)), new_cur)
  | '\001' => (Binary(Function(b)), new_cur)
  | '\004' => (Binary(UUID(b)), new_cur)
  | '\005' => (Binary(MD5(b)), new_cur)
  | '\128' => (Binary(UserDefined(b)), new_cur)
  | _ => raise(Malformed_bson)
  };
};

let decode_objectId = (str, cur) => (
  ObjectId(String.sub(str, cur, 12)),
  cur + 12,
);

let decode_boolean = (str, cur) => (
  Boolean(
    if (str.[cur] == '\000') {
      false;
    } else {
      true;
    },
  ),
  cur + 1,
);

let decode_utc = (str, cur) => {
  let (i, new_cur) = decode_int64(str, cur);
  (UTC(i), new_cur);
};

let decode_regex = (str, cur) => {
  let (s1, x00) = decode_cstring(str, cur);
  let (s2, new_cur) = decode_cstring(str, x00 + 1);
  ([@implicit_arity] Regex(s1, s2), new_cur);
};

let decode_jscode = (str, cur) => {
  let (s, next_cur) = decode_string(str, cur);
  (JSCode(s), next_cur);
};

let decode = str => {
  let rec decode_element = (str, cur) => {
    let c = str.[cur];
    let next_cur = cur + 1;
    let (ename, next_cur) = decode_ename(str, next_cur);
    /*print_endline ename;*/
    let (element, next_cur) =
      switch (c) {
      | '\001' => decode_double(str, next_cur)
      | '\002' =>
        /*print_endline "decoding string...";*/
        let (s, next_cur) = decode_string(str, next_cur);
        (String(s), next_cur);
      | '\003' =>
        let (doc, next_cur) = decode_doc(str, next_cur);
        (Document(doc), next_cur);
      | '\004' =>
        let (doc, next_cur) = decode_doc(str, next_cur);
        (Array(doc_to_list(doc)), next_cur);
      | '\005' => decode_binary(str, next_cur)
      | '\007' => decode_objectId(str, next_cur)
      | '\b' => decode_boolean(str, next_cur)
      | '\t' => decode_utc(str, next_cur)
      | '\n' => (Null(NULL), next_cur)
      | '\011' => decode_regex(str, next_cur)
      | '\r' => decode_jscode(str, next_cur)
      | '\015' =>
        /* decode jscode_w_s */
        let (_len, next_cur) = decode_len(str, next_cur);
        let (s, next_cur) = decode_string(str, next_cur);
        let (doc, next_cur) = decode_doc(str, next_cur);
        ([@implicit_arity] JSCodeWS(s, doc), next_cur);
      | '\016' =>
        let (i, next_cur) = decode_int32(str, next_cur);
        (Int32(i), next_cur);
      | '\017' =>
        let (i, next_cur) = decode_int64(str, next_cur);
        (Timestamp(i), next_cur);
      | '\018' =>
        let (i, next_cur) = decode_int64(str, next_cur);
        (Int64(i), next_cur);
      | '\255' => (MinKey(MINKEY), next_cur)
      | '\127' => (MaxKey(MAXKEY), next_cur)
      | _ => raise(Malformed_bson)
      };

    (ename, element, next_cur);
  }
  and decode_doc = (str, cur) => {
    let acc = empty;
    let (len, next_cur) = decode_len(str, cur);
    let rec decode_elements = (cur, acc) =>
      if (str.[cur] == '\000') {
        (acc, cur + 1);
      } else {
        let (ename, element, next_cur) = decode_element(str, cur);
        decode_elements(next_cur, add_element(ename, element, acc));
      };

    let (doc, des) = decode_elements(next_cur, acc);
    if (des - cur != len) {
      raise(Malformed_bson);
    } else {
      (doc, des);
    };
  };
  let (doc, _) = decode_doc(str, 0);
  doc;
};

/*
   Not that this bson to json conversion is far from completion.
   It is used to help the test verification and can handle only simple objects.
 */
let to_simple_json = doc => {
  let rec el_to_sl = el =>
    List.rev(List.fold_left((acc, e) => [e_to_s(e), ...acc], [], el))
  and e_to_s =
    fun
    | Double(v) => string_of_float(v)
    | String(v) => "\"" ++ v ++ "\""
    | Document(v) => d_to_s(v)
    | Array(v) => {
        let sl = el_to_sl(v);
        "[" ++ String.concat(", ", sl) ++ "]";
      }
    | Binary(v) =>
      switch (v) {
      | Generic(v)
      | Function(v)
      | UUID(v)
      | MD5(v)
      | UserDefined(v) => "\"" ++ v ++ "\""
      }
    | ObjectId(v) => "\"" ++ v ++ "\""
    | Boolean(v) => if (v) {"\"true\""} else {"\"false\""}
    | UTC(v) => Int64.to_string(v)
    | Null(NULL) => "\"null\""
    | [@implicit_arity] Regex(v1, v2) => "(\"" ++ v1 ++ ", \"" ++ v2 ++ "\")"
    | JSCode(v) => "\"" ++ v ++ "\""
    | [@implicit_arity] JSCodeWS(v, d) =>
      "(\"" ++ v ++ ", \"" ++ d_to_s(d) ++ "\")"
    | Int32(v) => Int32.to_string(v)
    | Timestamp(v) => Int64.to_string(v)
    | Int64(v) => Int64.to_string(v)
    | MinKey(MINKEY) => "\"minkey\""
    | MaxKey(MAXKEY) => "\"maxkey\""
    | _ => raise(Malformed_bson)
  and d_to_s = d => {
    let buf = Buffer.create(16);
    Buffer.add_string(buf, "{");
    /* let bindings = all_elements d in */
    let process = (acc, (ename, element)) => [
      "\"" ++ ename ++ "\" : " ++ e_to_s(element),
      ...acc,
    ];

    Buffer.add_string(
      buf,
      String.concat(", ", List.rev(List.fold_left(process, [], d))),
    );
    Buffer.add_string(buf, "}");
    Buffer.contents(buf);
  };

  d_to_s(doc);
};