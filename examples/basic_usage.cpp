json::Json user;
user["id"] = json::Json(123);
user["name"] = json::Json("Bob");
user["active"] = json::Json(true);
user["tags"] = json::Json();
user["tags"].add(json::Json("developer"));
user["tags"].add(json::Json("cpp"));

std::cout << user.to_string() << std::endl;
// 输出: {"active":true,"id":123,"name":"Bob","tags":["developer","cpp"]}
