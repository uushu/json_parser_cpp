
#include "jsonvalue.h"



//1.JsonNull
json::JsonType json::JsonNull::type() const
{
	return JsonType::NUL;
}

std::string json::JsonNull::to_string() const
{
	return "null";
}

std::unique_ptr <json::JsonValue> json::JsonNull::clone()const
{
	return std::make_unique<JsonNull>(*this);
}

//2.JsonBool
json::JsonType json::JsonBool::type()const
{
	return JsonType::BOOL;
}

std::string json::JsonBool::to_string() const
{
	return value ? "true" : "false";

}
std::unique_ptr<json::JsonValue> json::JsonBool::clone() const
{
	return std::make_unique<JsonBool>(*this);
}



//3.JsonString
json::JsonType json::JsonString::type()const
{
	return JsonType::STRING;
}
std::string json::JsonString::to_string()const
{
	return "\"" + value + "\""; // JSON�ַ�����Ҫ������"\""
}
const std::string& json::JsonString::get_string() const
{
	return this->value;
}
std::unique_ptr<json::JsonValue> json::JsonString::clone() const
{
	return std::make_unique<JsonString>(*this);
}


//4.JsonNumber
json::JsonType json::JsonNumber::type()const
{
	return JsonType::NUMBER;
}
std::string json::JsonNumber::to_string()const
{
	return std::to_string(value);
}
std::unique_ptr<json::JsonValue> json::JsonNumber::clone() const
{
	return std::make_unique<JsonNumber>(*this);
}


//5.JsonArray
json::JsonType json::JsonArray::type()const
{
	return JsonType::ARRAY;
}

std::string json::JsonArray::to_string() const
{
	//����
	std::string result = "[";

	for (size_t i = 0;i < values.size();i++)
	{
		result += values[i]->to_string();

		if (i < values.size() - 1)
		{
			result += ",";
		}

	}

	result += "]";
	return result;
}
std::unique_ptr<json::JsonValue> json::JsonArray::clone() const
{
	//����һ���µ�JsonArray����
	auto new_array = std::make_unique<json::JsonArray>();

	for (const auto& element_ptr : this->values)
	{
		new_array->add(element_ptr->clone());//��¡ÿ��Ԫ����ӵ���������
	}

	return new_array;

}
//���Ԫ�ص�����
void json::JsonArray::add(std::unique_ptr<json::JsonValue> value)
{
	//ʹ��std::move��value������Ȩ����vector��
	values.push_back(std::move(value));
}

//��ȡ������Ӧ�±��Ԫ��
//�������ö����ǿ�����ԭ��unique_ptr�������������ж�ռȨ
std::unique_ptr<json::JsonValue>& json::JsonArray::get(size_t index)
{
	//���ʵ�����������Χ�Զ�����
	if (index >= values.size())
	{
		values.resize(index + 1);
	}
	//���ݳ�����λ���ǿյģ���һ��ָ��JsonNull��unique_ptrָ������ӦԪ��
	if (!values[index])
	{
		values[index] = std::make_unique<JsonNull>();
	}
	return values[index];
}

//6.JsonObject
json::JsonType json::JsonObject::type()const
{
	return JsonType::OBJECT;
}
std::string json::JsonObject::to_string() const
{
	std::string result = "{";

	auto it = values.begin();
	while (it != values.end())
	{
		result += "\"" + it->first + "\":" + it->second->to_string();

		if (++it != values.end())
		{
			result += ",";
		}

	}

	result += "}";
	return result;
}
std::unique_ptr<json::JsonValue> json::JsonObject::clone() const
{
	auto new_object = std::make_unique<json::JsonObject>();
	for (const auto& pair : this->values)
	{
		new_object->set(pair.first, pair.second->clone()); //��¡ÿ����ֵ����ӵ��¶�����
	}
	return new_object;
}
//��Ӽ�ֵ�Ե�����
void json::JsonObject::set(const std::string& key, std::unique_ptr<JsonValue> value)
{
	values[key] = std::move(value);
}
std::unique_ptr<json::JsonValue>& json::JsonObject::get(const std::string& key)
{
	auto& ptr_ref = values[key];
	//���key�����ڣ�map���Զ�����һ��Ĭ��ֵ����unique_ptr����nullptr
	if(!ptr_ref)
	{
		ptr_ref = std::make_unique<JsonNull>();
	}
	return ptr_ref;
		
}