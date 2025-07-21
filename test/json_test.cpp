#include <gtest/gtest.h>
#include "json.h"
#include "jsonvalue.h"
#include "parser.h"
#include <stdexcept>

// ===== 基本类型测试 =====

TEST(JsonBasicTest, NullConstruction) {
    json::Json null_json;
    EXPECT_EQ(null_json.type(), json::JsonType::NUL);
    EXPECT_EQ(null_json.to_string(), "null");
}

TEST(JsonBasicTest, BoolConstruction) {
    json::Json true_json(true);
    json::Json false_json(false);

    EXPECT_EQ(true_json.type(), json::JsonType::BOOL);
    EXPECT_EQ(false_json.type(), json::JsonType::BOOL);
    EXPECT_EQ(true_json.to_string(), "true");
    EXPECT_EQ(false_json.to_string(), "false");
}

TEST(JsonBasicTest, NumberConstruction) {
    json::Json int_json(42);
    json::Json double_json(3.14);

    EXPECT_EQ(int_json.type(), json::JsonType::NUMBER);
    EXPECT_EQ(double_json.type(), json::JsonType::NUMBER);

    // 注意：std::to_string可能有精度问题，所以我们主要测试类型
    std::string int_str = int_json.to_string();
    std::string double_str = double_json.to_string();

    EXPECT_TRUE(int_str.find("42") != std::string::npos);
    EXPECT_TRUE(double_str.find("3.14") != std::string::npos);
}

TEST(JsonBasicTest, StringConstruction) {
    json::Json string_json("hello");
    json::Json cstr_json("world");
    std::string str = "test";
    json::Json stdstr_json(str);

    EXPECT_EQ(string_json.type(), json::JsonType::STRING);
    EXPECT_EQ(cstr_json.type(), json::JsonType::STRING);
    EXPECT_EQ(stdstr_json.type(), json::JsonType::STRING);

    EXPECT_EQ(string_json.to_string(), "\"hello\"");
    EXPECT_EQ(cstr_json.to_string(), "\"world\"");
    EXPECT_EQ(stdstr_json.to_string(), "\"test\"");

    EXPECT_EQ(string_json.get_string(), "hello");
    EXPECT_EQ(cstr_json.get_string(), "world");
    EXPECT_EQ(stdstr_json.get_string(), "test");
}

// ===== 数组操作测试 =====

TEST(JsonArrayTest, AddElements) {
    json::Json array;

    // 添加不同类型的元素
    array.add(json::Json(1));
    array.add(json::Json(2));
    array.add(json::Json("test"));
    array.add(json::Json(true));
    array.add(json::Json());  // null

    EXPECT_EQ(array.type(), json::JsonType::ARRAY);

    // 检查数组内容
    json::Json first = array[0];
    json::Json second = array[1];
    json::Json third = array[2];
    json::Json fourth = array[3];
    json::Json fifth = array[4];

    EXPECT_EQ(first.type(), json::JsonType::NUMBER);
    EXPECT_EQ(second.type(), json::JsonType::NUMBER);
    EXPECT_EQ(third.type(), json::JsonType::STRING);
    EXPECT_EQ(fourth.type(), json::JsonType::BOOL);
    EXPECT_EQ(fifth.type(), json::JsonType::NUL);

    EXPECT_EQ(third.get_string(), "test");
}

TEST(JsonArrayTest, IndexAssignment) {
    json::Json array;

    // 通过索引赋值
    array[0] = json::Json(10);
    array[1] = json::Json("hello");
    array[2] = json::Json(true);

    EXPECT_EQ(array.type(), json::JsonType::ARRAY);

    json::Json elem0 = array[0];
    json::Json elem1 = array[1];
    json::Json elem2 = array[2];

    EXPECT_EQ(elem0.type(), json::JsonType::NUMBER);
    EXPECT_EQ(elem1.type(), json::JsonType::STRING);
    EXPECT_EQ(elem2.type(), json::JsonType::BOOL);

    EXPECT_EQ(elem1.get_string(), "hello");
}

TEST(JsonArrayTest, AutoExpansion) {
    json::Json array;

    // 测试数组自动扩展
    array[5] = json::Json("far");

    EXPECT_EQ(array.type(), json::JsonType::ARRAY);

    // 检查中间的元素是否为null
    json::Json middle = array[2];
    EXPECT_EQ(middle.type(), json::JsonType::NUL);

    json::Json far = array[5];
    EXPECT_EQ(far.get_string(), "far");
}

// ===== 对象操作测试 =====

TEST(JsonObjectTest, KeyValuePairs) {
    json::Json obj;

    // 设置键值对
    obj["name"] = json::Json("John");
    obj["age"] = json::Json(30);
    obj["active"] = json::Json(true);
    obj["score"] = json::Json(95.5);
    obj["data"] = json::Json();  // null

    EXPECT_EQ(obj.type(), json::JsonType::OBJECT);

    // 验证值
    json::Json name = obj["name"];
    json::Json age = obj["age"];
    json::Json active = obj["active"];
    json::Json score = obj["score"];
    json::Json data = obj["data"];

    EXPECT_EQ(name.type(), json::JsonType::STRING);
    EXPECT_EQ(age.type(), json::JsonType::NUMBER);
    EXPECT_EQ(active.type(), json::JsonType::BOOL);
    EXPECT_EQ(score.type(), json::JsonType::NUMBER);
    EXPECT_EQ(data.type(), json::JsonType::NUL);

    EXPECT_EQ(name.get_string(), "John");
}

// ===== 嵌套结构测试 =====

TEST(JsonNestedTest, ComplexStructure) {
    json::Json root;

    // 创建复杂的嵌套结构
    root["user"]["name"] = json::Json("Alice");
    root["user"]["details"]["age"] = json::Json(25);
    root["user"]["details"]["city"] = json::Json("New York");

    // 创建数组
    root["user"]["hobbies"] = json::Json();
    root["user"]["hobbies"].add(json::Json("reading"));
    root["user"]["hobbies"].add(json::Json("coding"));
    root["user"]["hobbies"].add(json::Json("gaming"));

    // 验证结构
    EXPECT_EQ(root.type(), json::JsonType::OBJECT);

    json::Json user = root["user"];
    EXPECT_EQ(user.type(), json::JsonType::OBJECT);

    json::Json name = user["name"];
    EXPECT_EQ(name.get_string(), "Alice");

    json::Json details = user["details"];
    EXPECT_EQ(details.type(), json::JsonType::OBJECT);

    json::Json age = details["age"];
    EXPECT_EQ(age.type(), json::JsonType::NUMBER);

    json::Json city = details["city"];
    EXPECT_EQ(city.get_string(), "New York");

    json::Json hobbies = user["hobbies"];
    EXPECT_EQ(hobbies.type(), json::JsonType::ARRAY);

    json::Json first_hobby = hobbies[0];
    EXPECT_EQ(first_hobby.get_string(), "reading");

    json::Json second_hobby = hobbies[1];
    EXPECT_EQ(second_hobby.get_string(), "coding");
}

// ===== 拷贝和移动测试 =====

TEST(JsonCopyMoveTest, CopyConstructor) {
    json::Json original("test_value");
    json::Json copy(original);

    EXPECT_EQ(copy.type(), json::JsonType::STRING);
    EXPECT_EQ(copy.get_string(), "test_value");
    EXPECT_EQ(original.get_string(), "test_value");  // 原始对象应该不变
}

TEST(JsonCopyMoveTest, CopyAssignment) {
    json::Json original(42);
    json::Json copy;
    copy = original;

    EXPECT_EQ(copy.type(), json::JsonType::NUMBER);
    EXPECT_EQ(original.type(), json::JsonType::NUMBER);
}

TEST(JsonCopyMoveTest, MoveConstructor) {
    json::Json original("move_test");
    json::Json moved(std::move(original));

    EXPECT_EQ(moved.type(), json::JsonType::STRING);
    EXPECT_EQ(moved.get_string(), "move_test");
}

// ===== JSON解析测试 =====

TEST(JsonParserTest, ParseNull) {
    std::string json_str = "null";
    json::Json parsed = json::Json::parse(json_str);

    EXPECT_EQ(parsed.type(), json::JsonType::NUL);
    EXPECT_EQ(parsed.to_string(), "null");
}

TEST(JsonParserTest, ParseBool) {
    json::Json true_parsed = json::Json::parse("true");
    json::Json false_parsed = json::Json::parse("false");

    EXPECT_EQ(true_parsed.type(), json::JsonType::BOOL);
    EXPECT_EQ(false_parsed.type(), json::JsonType::BOOL);
    EXPECT_EQ(true_parsed.to_string(), "true");
    EXPECT_EQ(false_parsed.to_string(), "false");
}

TEST(JsonParserTest, ParseNumber) {
    json::Json int_parsed = json::Json::parse("42");
    json::Json double_parsed = json::Json::parse("3.14");
    json::Json negative_parsed = json::Json::parse("-10");

    EXPECT_EQ(int_parsed.type(), json::JsonType::NUMBER);
    EXPECT_EQ(double_parsed.type(), json::JsonType::NUMBER);
    EXPECT_EQ(negative_parsed.type(), json::JsonType::NUMBER);
}

TEST(JsonParserTest, ParseString) {
    json::Json simple_parsed = json::Json::parse("\"hello\"");
    json::Json escaped_parsed = json::Json::parse("\"hello\\nworld\"");

    EXPECT_EQ(simple_parsed.type(), json::JsonType::STRING);
    EXPECT_EQ(escaped_parsed.type(), json::JsonType::STRING);
    EXPECT_EQ(simple_parsed.get_string(), "hello");
    EXPECT_EQ(escaped_parsed.get_string(), "hello\nworld");
}

TEST(JsonParserTest, ParseArray) {
    std::string array_str = "[1, 2, \"test\", true, null]";
    json::Json parsed = json::Json::parse(array_str);

    EXPECT_EQ(parsed.type(), json::JsonType::ARRAY);

    json::Json first = parsed[0];
    json::Json second = parsed[1];
    json::Json third = parsed[2];
    json::Json fourth = parsed[3];
    json::Json fifth = parsed[4];

    EXPECT_EQ(first.type(), json::JsonType::NUMBER);
    EXPECT_EQ(second.type(), json::JsonType::NUMBER);
    EXPECT_EQ(third.type(), json::JsonType::STRING);
    EXPECT_EQ(fourth.type(), json::JsonType::BOOL);
    EXPECT_EQ(fifth.type(), json::JsonType::NUL);

    EXPECT_EQ(third.get_string(), "test");
}

TEST(JsonParserTest, ParseObject) {
    std::string obj_str = "{\"name\": \"John\", \"age\": 30, \"active\": true}";
    json::Json parsed = json::Json::parse(obj_str);

    EXPECT_EQ(parsed.type(), json::JsonType::OBJECT);

    json::Json name = parsed["name"];
    json::Json age = parsed["age"];
    json::Json active = parsed["active"];

    EXPECT_EQ(name.type(), json::JsonType::STRING);
    EXPECT_EQ(age.type(), json::JsonType::NUMBER);
    EXPECT_EQ(active.type(), json::JsonType::BOOL);

    EXPECT_EQ(name.get_string(), "John");
}

TEST(JsonParserTest, ParseNestedStructure) {
    std::string nested_str = R"({
        "user": {
            "name": "Alice",
            "details": {
                "age": 25,
                "city": "New York"
            },
            "hobbies": ["reading", "coding", "gaming"]
        },
        "active": true,
        "version": 1.0
    })";

    json::Json parsed = json::Json::parse(nested_str);

    EXPECT_EQ(parsed.type(), json::JsonType::OBJECT);

    json::Json user = parsed["user"];
    EXPECT_EQ(user.type(), json::JsonType::OBJECT);

    json::Json name = user["name"];
    EXPECT_EQ(name.get_string(), "Alice");

    json::Json details = user["details"];
    EXPECT_EQ(details.type(), json::JsonType::OBJECT);

    json::Json age = details["age"];
    EXPECT_EQ(age.type(), json::JsonType::NUMBER);

    json::Json city = details["city"];
    EXPECT_EQ(city.get_string(), "New York");

    json::Json hobbies = user["hobbies"];
    EXPECT_EQ(hobbies.type(), json::JsonType::ARRAY);

    json::Json first_hobby = hobbies[0];
    EXPECT_EQ(first_hobby.get_string(), "reading");

    json::Json active = parsed["active"];
    EXPECT_EQ(active.type(), json::JsonType::BOOL);
}

// ===== 错误处理测试 =====

TEST(JsonErrorTest, ParseErrors) {
    // 测试无效JSON应该抛出异常
    EXPECT_THROW(json::Json::parse("invalid"), std::logic_error);
    EXPECT_THROW(json::Json::parse("{invalid}"), std::logic_error);
    EXPECT_THROW(json::Json::parse("\"unclosed string"), std::logic_error);
    EXPECT_THROW(json::Json::parse("[1,2,"), std::logic_error);
}

TEST(JsonErrorTest, GetStringFromNonString) {
    json::Json number_json(42);
    json::Json bool_json(true);
    json::Json null_json;

    // 尝试从非字符串JSON获取字符串应该抛出异常
    EXPECT_THROW(number_json.get_string(), std::logic_error);
    EXPECT_THROW(bool_json.get_string(), std::logic_error);
    EXPECT_THROW(null_json.get_string(), std::logic_error);
}

// ===== 边界情况测试 =====

TEST(JsonEdgeCaseTest, EmptyStructures) {
    json::Json empty_array_parsed = json::Json::parse("[]");
    json::Json empty_object_parsed = json::Json::parse("{}");

    EXPECT_EQ(empty_array_parsed.type(), json::JsonType::ARRAY);
    EXPECT_EQ(empty_object_parsed.type(), json::JsonType::OBJECT);
}

TEST(JsonEdgeCaseTest, WhitespaceHandling) {
    std::string whitespace_json = "  \n\t  { \n \"key\" \t : \n \"value\" \t } \n  ";
    json::Json parsed = json::Json::parse(whitespace_json);

    EXPECT_EQ(parsed.type(), json::JsonType::OBJECT);
    json::Json value = parsed["key"];
    EXPECT_EQ(value.get_string(), "value");
}

// ===== 序列化测试 =====

TEST(JsonSerializationTest, BasicSerialization) {
    json::Json obj;
    obj["name"] = json::Json("John");
    obj["age"] = json::Json(30);

    std::string serialized = obj.to_string();

    // 序列化后的字符串应该包含键值对
    EXPECT_TRUE(serialized.find("\"name\"") != std::string::npos);
    EXPECT_TRUE(serialized.find("\"John\"") != std::string::npos);
    EXPECT_TRUE(serialized.find("\"age\"") != std::string::npos);
}

// ===== 性能测试 =====

TEST(JsonPerformanceTest, LargeArray) {
    json::Json large_array;

    // 创建一个较大的数组
    for (int i = 0; i < 100; ++i) {
        large_array.add(json::Json(i));
    }

    EXPECT_EQ(large_array.type(), json::JsonType::ARRAY);

    // 验证几个元素
    json::Json first = large_array[0];
    json::Json middle = large_array[50];
    json::Json last = large_array[99];

    EXPECT_EQ(first.type(), json::JsonType::NUMBER);
    EXPECT_EQ(middle.type(), json::JsonType::NUMBER);
    EXPECT_EQ(last.type(), json::JsonType::NUMBER);
}

 //主函数
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    std::cout << "Running JSON Parser Tests..." << std::endl;
    std::cout << "========================================" << std::endl;

    int result = RUN_ALL_TESTS();

    if (result == 0) {
        std::cout << "\n All tests passed! Your JSON parser is working correctly!" << std::endl;
    }
    else {
        std::cout << "\n Some tests failed. Check the output above for details." << std::endl;
    }

    return result;
}
