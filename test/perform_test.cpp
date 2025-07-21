#include <iostream>
#include <string>
#include <vector>
#include <chrono> // 用于计时的头文件
#include "json.h"


//int main() {
//    // 1. 准备一个有一定规模和复杂度的测试样本
//    std::string large_json_str = R"({
//        "glossary": {
//            "title": "example glossary",
//            "GlossDiv": {
//                "title": "S",
//                "GlossList": {
//                    "GlossEntry": {
//                        "ID": "SGML",
//                        "SortAs": "SGML",
//                        "GlossTerm": "Standard Generalized Markup Language",
//                        "Acronym": "SGML",
//                        "Abbrev": "ISO 8879:1986",
//                        "GlossDef": {
//                            "para": "A meta-markup language, used to create markup languages such as DocBook.",
//                            "GlossSeeAlso": ["GML", "XML"]
//                        },
//                        "GlossSee": "markup"
//                    }
//                }
//            }
//        },
//        "numbers": [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1e5, -0.123]
//    })";
//
//    const int RUN_COUNT = 100; // 运行100次取平均值，让结果更稳定
//
//    // --- 2. 测试解析性能 ---
//    std::cout << "--- Testing Parser Performance ---" << std::endl;
//
//    // 获取开始时间点
//    auto start_time_parse = std::chrono::high_resolution_clock::now();
//
//    json::Json parsed_data;
//    for (int i = 0; i < RUN_COUNT; ++i) {
//        parsed_data = json::Json::parse(large_json_str);
//    }
//
//    // 获取结束时间点
//    auto end_time_parse = std::chrono::high_resolution_clock::now();
//    // 计算总耗时
//    auto duration_parse = std::chrono::duration_cast<std::chrono::microseconds>(end_time_parse - start_time_parse);
//
//    std::cout << "Total time for " << RUN_COUNT << " parses: " << duration_parse.count() << " microseconds." << std::endl;
//    std::cout << "Average time per parse: " << duration_parse.count() / RUN_COUNT << " microseconds." << std::endl;
//
//
//    // --- 3. 测试序列化性能 ---
//    std::cout << "\n--- Testing Serialization Performance ---" << std::endl;
//
//    auto start_time_serialize = std::chrono::high_resolution_clock::now();
//
//    std::string serialized_str;
//    for (int i = 0; i < RUN_COUNT; ++i) {
//        serialized_str = parsed_data.to_string();
//    }
//
//    auto end_time_serialize = std::chrono::high_resolution_clock::now();
//    auto duration_serialize = std::chrono::duration_cast<std::chrono::microseconds>(end_time_serialize - start_time_serialize);
//
//    std::cout << "Total time for " << RUN_COUNT << " serializations: " << duration_serialize.count() << " microseconds." << std::endl;
//    std::cout << "Average time per serialization: " << duration_serialize.count() / RUN_COUNT << " microseconds." << std::endl;
//
//    return 0;
//}
