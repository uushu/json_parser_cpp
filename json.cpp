#include"jsonvalue.h"
#include "json.h"

//默认构造函数->JsonNull
json::Json::Json()
{
	value_ptr = std::make_unique<json::JsonNull>();
}

//JsonNumber
json::Json::Json(int value)
{
	value_ptr = std::make_unique<json::JsonNumber>(value);
}
json::Json::Json(double value)
{
	value_ptr = std::make_unique<json::JsonNumber>(value);
}

//JsonBool
json::Json::Json(bool value)
{
	value_ptr = std::make_unique<json::JsonBool>(value);
}

//string构造函数->JsonString
json::Json::Json(const std::string& value)
{
	value_ptr = std::make_unique<json::JsonString>(value);
}

//C风格字符串 构造函数->JsonString
json::Json::Json(const char* value)//字符串字面值常量
{
	value_ptr = std::make_unique<json::JsonString>(std::string(value));
}
//新增的构造函数
json::Json::Json(std::unique_ptr <json::JsonValue> ptr)
{
	value_ptr = std::move(ptr);
}

json::Json::~Json()
{

}

//拷贝构造函数用clone()进行深拷贝
json::Json::Json(const json::Json& other)//拷贝构造
{
	this->value_ptr = other.value_ptr ? other.value_ptr->clone() : nullptr;
}

//拷贝赋值使用 copy and swap 
json::Json& json::Json::operator=(const Json& other)//拷贝赋值
{
	Json temp(other);//copy
	std::swap(this->value_ptr, temp.value_ptr);//交换内部指针
	return *this;

}

//移动构造
json::Json::Json(Json&& other) noexcept//移动构造函数
{
	//拿走other的指针，让other指针失效
	this->value_ptr = std::move(other.value_ptr);
}

//移动赋值
json::Json& json::Json::operator=(Json&& other) noexcept//移动赋值
{
	//直接交换指针
	std::swap(this->value_ptr, other.value_ptr);
	return *this;
}


//序列化

std::string json::Json::to_string() const
{
	return value_ptr->to_string();
}

//获取原始数据
std::string json::Json::get_string() const
{
	auto* str_ptr = dynamic_cast<JsonString*>(value_ptr.get());
	if (str_ptr)
	{
		return str_ptr->get_string();

	}
	throw std::logic_error("Json value is not a string.");
}

//获取类型
json::JsonType json::Json::type() const
{
	return value_ptr->type();
}




std::unique_ptr<json::JsonValue> json::Json::clone()const
{
	//如果内部指针有效就调用它指向的的对象clone()方法，否则返回一个空指针
	return value_ptr ? value_ptr->clone() : nullptr;
}



//向数组添加元素

//传入左值
void json::Json::add(const Json& value)
{
	//先检查它是不是数组,不是就把它变成空数组
	if (this->type() != json::JsonType::ARRAY)
	{
		this->value_ptr = std::make_unique<json::JsonArray>();
	}
	//已经确定是数组后使用dynamic_cast 将基类指针安全转化为派生类指针
	auto* arr_ptr = dynamic_cast<json::JsonArray*>(this->value_ptr.get());

	if (arr_ptr)
	{
		arr_ptr->add(value.clone());
	}

}

//移动(右值）
void json::Json::add(Json&& value)
{
	if (this->type() != json::JsonType::ARRAY)
	{
		this->value_ptr = std::make_unique<json::JsonArray>();
	}
	auto* arr_ptr = dynamic_cast<json::JsonArray*>(this->value_ptr.get());
	if (arr_ptr)
	{
		arr_ptr->add(std::move(value.value_ptr));
	}
}

 json::Json json::Json::parse(const std::string& content)
 {
	 Parser p(content);

	 return p.parse();

 }




//1. 用于数组访问的 operator[]
json::Json::JsonProxy json::Json::operator[](size_t index)
{
	if (type() != json::JsonType::ARRAY)
	{
		*this = Json(std::make_unique<json::JsonArray>());
	}
	auto* arr_ptr = dynamic_cast<json::JsonArray*>(value_ptr.get());
	return JsonProxy(arr_ptr->get(index));
}

//2. 用于对象访问的 operator[]
json::Json::JsonProxy json::Json::operator[](const std::string& key)
{
	if (type() != json::JsonType::OBJECT)
	{
		*this = Json(std::make_unique<json::JsonObject > ());

	}
	auto* obj_ptr = dynamic_cast<json::JsonObject*>(value_ptr.get());
	return JsonProxy(obj_ptr->get(key));
}

//输出标准Json格式字符串
std::ostream& json::operator<<(std::ostream& os, const Json& json)
{
	os << json.to_string();
	return os;
}
