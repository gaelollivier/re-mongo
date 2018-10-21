exception InvalidObjectId;

type t = string;

Random.self_init();

let processUnique1 = Random.int(0xffffff);
let processUnique2 = Random.int(0xffff);

let counter = ref(Random.int(0xffffff));

let fromBinaryString = id =>
  if (String.length(id) != 12) {
    raise(InvalidObjectId);
  } else {
    id;
  };

let toBinaryString = id => id;

let toString = id => Hex.show(Hex.of_string(id));

let generate = () => {
  /* NOTE: ObjectId timestamp and counter are encoded in big-endian, to allow
     simple sorting by comparing byte-by-byte.
     See https://stackoverflow.com/questions/23539486/endianess-of-parts-of-on-objectid-in-bson */
  let buffer = Buffer.create(12);
  let now = int_of_float(Unix.time());

  /* 4 bytes timestamp */
  Buffer.add_char(buffer, char_of_int(now lsr 24 land 0xff));
  Buffer.add_char(buffer, char_of_int(now lsr 16 land 0xff));
  Buffer.add_char(buffer, char_of_int(now lsr 8 land 0xff));
  Buffer.add_char(buffer, char_of_int(now land 0xff));

  /* 5 byte process unique */
  Buffer.add_char(buffer, char_of_int(processUnique1 land 0xff));
  Buffer.add_char(buffer, char_of_int(processUnique1 lsr 8 land 0xff));
  Buffer.add_char(buffer, char_of_int(processUnique1 lsr 16 land 0xff));
  Buffer.add_char(buffer, char_of_int(processUnique2 land 0xff));
  Buffer.add_char(buffer, char_of_int(processUnique2 lsr 8 land 0xff));

  /* 3 bytes counter */
  Buffer.add_char(buffer, char_of_int(counter^ lsr 16 land 0xff));
  Buffer.add_char(buffer, char_of_int(counter^ lsr 8 land 0xff));
  Buffer.add_char(buffer, char_of_int(counter^ land 0xff));

  /* increment counter */
  counter := (counter^ + 1) mod 0xffffff;

  Buffer.contents(buffer);
};