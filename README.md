# JSON Parser C++

一个现代化的C++ JSON解析库，采用智能指针内存管理和面向对象设计。

## ✨ 特性

- 🚀 **高性能** - 基于递归下降解析器，支持移动语义
- 🛡️ **内存安全** - 使用智能指针，零内存泄漏
- 🎯 **类型安全** - 编译时类型检查，运行时异常保护
- 🔗 **链式操作** - 支持 `json["user"]["name"]` 语法
- 📝 **标准兼容** - 完全符合JSON规范
- 🧪 **完整测试** - 25个单元测试，覆盖率100%

## 📋 支持的JSON类型

- ✅ **Object** `{"key": "value"}`
- ✅ **Array** `[1, 2, 3]`
- ✅ **String** `"hello world"`
- ✅ **Number** `42`, `3.14`
- ✅ **Boolean** `true`, `false`
- ✅ **Null** `null`

## 🚀 快速开始

### 基本用法

```cpp
#include "json.h"
#include <iostream>

int main() {
    // 解析JSON字符串
    std::string jsonStr = R"({
        "name": "Alice",
        "age": 25,
        "hobbies": ["reading", "coding"]
    })";
    
    json::Json data = json::Json::parser(jsonStr);
    
    // 访问数据
    std::string name = data["name"].get_string();
    std::cout << "Name: " << name << std::endl;
    
    // 修改数据
    data["age"] = json::Json(26);
    data["hobbies"].add(json::Json("gaming"));
    
    // 序列化输出
    std::cout << data.to_string() << std::endl;
    
    return 0;
}
