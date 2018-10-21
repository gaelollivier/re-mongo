/* cf: http://docs.mongodb.org/manual/reference/operator/ */

type meta_op =
  | Comment(string)
  | MaxScan(int)
  | Max(Bson.t)
  | Min(Bson.t)
  | OrderBy(Bson.t)
  | Explain
  | Hint(Bson.t)
  | ReturnKey
  | ShowDiskLoc
  | Snapshot;

let meta_op = (query, op) => {
  let r =
    if (Bson.hasElement("$query", query)) {
      query;
    } else {
      Bson.addElement("$query", Bson.createDocElement(query), Bson.empty);
    };

  switch (op) {
  | Comment(c) => Bson.addElement("$comment", Bson.createString(c), r)
  | MaxScan(i) =>
    Bson.addElement("$maxScan", Bson.createInt32(Int32.of_int(i)), r)
  | Max(max_bson) =>
    Bson.addElement("$max", Bson.createDocElement(max_bson), r)
  | Min(min_bson) =>
    Bson.addElement("$min", Bson.createDocElement(min_bson), r)
  | OrderBy(orderby_bson) =>
    Bson.addElement("$orderby", Bson.createDocElement(orderby_bson), r)
  | Explain => Bson.addElement("$explain", Bson.createInt32(1l), r)
  | Hint(hint_bson) =>
    Bson.addElement("$hint", Bson.createDocElement(hint_bson), r)
  | ReturnKey => Bson.addElement("$returnKey", Bson.createBoolean(true), r)
  | ShowDiskLoc =>
    Bson.addElement("$showDiskLoc", Bson.createBoolean(true), r)
  | Snapshot => Bson.addElement("$snapshot", Bson.createBoolean(true), r)
  };
};

let comment = (c, query) => meta_op(query, Comment(c));

let maxScan = (i, query) => meta_op(query, MaxScan(i));

let min = (i, query) => meta_op(query, Min(i));

let max = (i, query) => meta_op(query, Max(i));

let orderBy = (o, query) => meta_op(query, OrderBy(o));

let explain = query => meta_op(query, Explain);

let hint = (h, query) => meta_op(query, Hint(h));

let returnKey = query => meta_op(query, ReturnKey);

let showDiskLoc = query => meta_op(query, ShowDiskLoc);

let snapshot = query => meta_op(query, Snapshot);