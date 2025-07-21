cmake_minimum_required(VERSION 3.10)
project(JsonParserCpp VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含目录
include_directories(include)

# 创建库
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")

add_library(json_parser ${SOURCES} ${HEADERS})

# 设置头文件目录
target_include_directories(json_parser PUBLIC include)

# 示例程序
add_subdirectory(examples)

# 测试
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# 安装
install(TARGETS json_parser
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/json_parser)
