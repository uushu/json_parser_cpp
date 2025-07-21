#include <iostream>
#include <string>
#include <vector>
#include <chrono> // ���ڼ�ʱ��ͷ�ļ�
#include "json.h"


//int main() {
//    // 1. ׼��һ����һ����ģ�͸��ӶȵĲ�������
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
//    const int RUN_COUNT = 100; // ����100��ȡƽ��ֵ���ý�����ȶ�
//
//    // --- 2. ���Խ������� ---
//    std::cout << "--- Testing Parser Performance ---" << std::endl;
//
//    // ��ȡ��ʼʱ���
//    auto start_time_parse = std::chrono::high_resolution_clock::now();
//
//    json::Json parsed_data;
//    for (int i = 0; i < RUN_COUNT; ++i) {
//        parsed_data = json::Json::parse(large_json_str);
//    }
//
//    // ��ȡ����ʱ���
//    auto end_time_parse = std::chrono::high_resolution_clock::now();
//    // �����ܺ�ʱ
//    auto duration_parse = std::chrono::duration_cast<std::chrono::microseconds>(end_time_parse - start_time_parse);
//
//    std::cout << "Total time for " << RUN_COUNT << " parses: " << duration_parse.count() << " microseconds." << std::endl;
//    std::cout << "Average time per parse: " << duration_parse.count() / RUN_COUNT << " microseconds." << std::endl;
//
//
//    // --- 3. �������л����� ---
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