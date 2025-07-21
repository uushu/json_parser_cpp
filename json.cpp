#include"jsonvalue.h"
#include "json.h"

//Ĭ�Ϲ��캯��->JsonNull
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

//string���캯��->JsonString
json::Json::Json(const std::string& value)
{
	value_ptr = std::make_unique<json::JsonString>(value);
}

//C����ַ��� ���캯��->JsonString
json::Json::Json(const char* value)//�ַ�������ֵ����
{
	value_ptr = std::make_unique<json::JsonString>(std::string(value));
}
//�����Ĺ��캯��
json::Json::Json(std::unique_ptr <json::JsonValue> ptr)
{
	value_ptr = std::move(ptr);
}

json::Json::~Json()
{

}

//�������캯����clone()�������
json::Json::Json(const json::Json& other)//��������
{
	this->value_ptr = other.value_ptr ? other.value_ptr->clone() : nullptr;
}

//������ֵʹ�� copy and swap 
json::Json& json::Json::operator=(const Json& other)//������ֵ
{
	Json temp(other);//copy
	std::swap(this->value_ptr, temp.value_ptr);//�����ڲ�ָ��
	return *this;

}

//�ƶ�����
json::Json::Json(Json&& other) noexcept//�ƶ����캯��
{
	//����other��ָ�룬��otherָ��ʧЧ
	this->value_ptr = std::move(other.value_ptr);
}

//�ƶ���ֵ
json::Json& json::Json::operator=(Json&& other) noexcept//�ƶ���ֵ
{
	//ֱ�ӽ���ָ��
	std::swap(this->value_ptr, other.value_ptr);
	return *this;
}


//���л�

std::string json::Json::to_string() const
{
	return value_ptr->to_string();
}

//��ȡԭʼ����
std::string json::Json::get_string() const
{
	auto* str_ptr = dynamic_cast<JsonString*>(value_ptr.get());
	if (str_ptr)
	{
		return str_ptr->get_string();

	}
	throw std::logic_error("Json value is not a string.");
}

//��ȡ����
json::JsonType json::Json::type() const
{
	return value_ptr->type();
}




std::unique_ptr<json::JsonValue> json::Json::clone()const
{
	//����ڲ�ָ����Ч�͵�����ָ��ĵĶ���clone()���������򷵻�һ����ָ��
	return value_ptr ? value_ptr->clone() : nullptr;
}



//���������Ԫ��

//������ֵ
void json::Json::add(const Json& value)
{
	//�ȼ�����ǲ�������,���ǾͰ�����ɿ�����
	if (this->type() != json::JsonType::ARRAY)
	{
		this->value_ptr = std::make_unique<json::JsonArray>();
	}
	//�Ѿ�ȷ���������ʹ��dynamic_cast ������ָ�밲ȫת��Ϊ������ָ��
	auto* arr_ptr = dynamic_cast<json::JsonArray*>(this->value_ptr.get());

	if (arr_ptr)
	{
		arr_ptr->add(value.clone());
	}

}

//�ƶ�(��ֵ��
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




//1. ����������ʵ� operator[]
json::Json::JsonProxy json::Json::operator[](size_t index)
{
	if (type() != json::JsonType::ARRAY)
	{
		*this = Json(std::make_unique<json::JsonArray>());
	}
	auto* arr_ptr = dynamic_cast<json::JsonArray*>(value_ptr.get());
	return JsonProxy(arr_ptr->get(index));
}

//2. ���ڶ�����ʵ� operator[]
json::Json::JsonProxy json::Json::operator[](const std::string& key)
{
	if (type() != json::JsonType::OBJECT)
	{
		*this = Json(std::make_unique<json::JsonObject > ());

	}
	auto* obj_ptr = dynamic_cast<json::JsonObject*>(value_ptr.get());
	return JsonProxy(obj_ptr->get(key));
}

//�����׼Json��ʽ�ַ���
std::ostream& json::operator<<(std::ostream& os, const Json& json)
{
	os << json.to_string();
	return os;
}
