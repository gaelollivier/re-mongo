/*** Logical Query operator  */

let or_op = d_list =>
  Bson.addElement("$or", Bson.createDocElementList(d_list), Bson.empty);

let and_op = d_list =>
  Bson.addElement("$and", Bson.createDocElementList(d_list), Bson.empty);

let not_op = e => Bson.addElement("$not", e, Bson.empty);

let nor_op = d_list =>
  Bson.addElement("$nor", Bson.createDocElementList(d_list), Bson.empty);

/*** Query Operator Array */

let all = e_list =>
  Bson.addElement("$all", Bson.createList(e_list), Bson.empty);

let elemMatch = d =>
  Bson.addElement("$elemMatch", Bson.createDocElement(d), Bson.empty);

let size = i =>
  Bson.addElement("$size", Bson.createInt32(Int32.of_int(i)), Bson.empty);

/*** Field Update Operator  */

let inc = d => Bson.addElement("$inc", Bson.createDocElement(d), Bson.empty);

let rename = d =>
  Bson.addElement("$rename", Bson.createDocElement(d), Bson.empty);

let setOnInsert = d =>
  Bson.addElement("$setOnInsert", Bson.createDocElement(d), Bson.empty);

let set = d => Bson.addElement("$set", Bson.createDocElement(d), Bson.empty);

let unset = d =>
  Bson.addElement("$unset", Bson.createDocElement(d), Bson.empty);

/*** Array Update Operator  */

let addToSet = d =>
  Bson.addElement("$addToSet", Bson.createDocElement(d), Bson.empty);

let pop = d => Bson.addElement("$pop", Bson.createDocElement(d), Bson.empty);

let pullAll = d =>
  Bson.addElement("$pullAll", Bson.createDocElement(d), Bson.empty);

let pull = d =>
  Bson.addElement("$pull", Bson.createDocElement(d), Bson.empty);

let pushAll = d =>
  Bson.addElement("$pushAll", Bson.createDocElement(d), Bson.empty);

let push = d =>
  Bson.addElement("$push", Bson.createDocElement(d), Bson.empty);

let each = el => Bson.addElement("$each", Bson.createList(el), Bson.empty);

let slice = i =>
  Bson.addElement("$slice", Bson.createInt32(Int32.of_int(i)), Bson.empty);

let sort = d =>
  Bson.addElement("$sort", Bson.createDocElement(d), Bson.empty);