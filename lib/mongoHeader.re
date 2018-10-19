open MongoUtils;

type t = {
  message_len: int32,
  request_id: int32,
  response_to: int32,
  op: MongoOperation.t,
};

let create_header = (body_len, request_id, response_to, op) => {
  message_len: Int32.of_int(body_len + 4 * 4),
  request_id,
  response_to,
  op,
};

let create_request_header = (body_len, request_id, op) => {
  message_len: Int32.of_int(body_len + 4 * 4),
  request_id,
  response_to: 0l,
  op,
};

let get_message_len = h => h.message_len;
let get_request_id = h => h.request_id;
let get_response_to = h => h.response_to;
let get_op = h => h.op;

let encode_header = h => {
  let buf = Buffer.create(8);
  encode_int32(buf, h.message_len);
  encode_int32(buf, h.request_id);
  encode_int32(buf, h.response_to);
  encode_int32(buf, MongoOperation.to_code(h.op));
  Buffer.contents(buf);
};

let decode_header = str => {
  let (message_len, next) = decode_int32(str, 0);
  let (request_id, next) = decode_int32(str, next);
  let (response_to, next) = decode_int32(str, next);
  let (op_code, _next) = decode_int32(str, next);
  {
    message_len,
    request_id,
    response_to,
    op: MongoOperation.of_code(op_code),
  };
};

let to_string = h => {
  let buf = Buffer.create(64);
  Buffer.add_string(buf, "message_len = ");
  Buffer.add_string(buf, Int32.to_string(h.message_len));
  Buffer.add_string(buf, "\n");
  Buffer.add_string(buf, "request_id = ");
  Buffer.add_string(buf, Int32.to_string(h.request_id));
  Buffer.add_string(buf, "\n");
  Buffer.add_string(buf, "response_to = ");
  Buffer.add_string(buf, Int32.to_string(h.response_to));
  Buffer.add_string(buf, "\n");
  Buffer.add_string(buf, "op = ");
  Buffer.add_string(buf, Int32.to_string(MongoOperation.to_code(h.op)));
  Buffer.add_string(buf, "\n");
  Buffer.contents(buf);
};
