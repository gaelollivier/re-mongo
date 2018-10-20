type t = string;

Random.self_init();

let processUnique1 = Random.int(0xffffffff);
let processUnique2 = Random.int(0xff);

let counter = ref(Random.int(0xffffff));

/* Generate a new (probably) unique ObjectId */
let generate = (): t => {
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
  Buffer.add_char(buffer, char_of_int(processUnique1 lsr 24 land 0xff));
  Buffer.add_char(buffer, char_of_int(processUnique2 land 0xff));

  /* 3 bytes counter */
  Buffer.add_char(buffer, char_of_int(counter^ lsr 16 land 0xff));
  Buffer.add_char(buffer, char_of_int(counter^ lsr 8 land 0xff));
  Buffer.add_char(buffer, char_of_int(counter^ land 0xff));

  /* increment counter */
  counter := (counter^ + 1) mod 0xffffff;

  Buffer.contents(buffer);
};