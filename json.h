#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "jsonvalue.h"

#include"parser.h"

namespace json
{

	//����һ���û�����ֱ��ʹ�õ�����json��
	class Json
	{

	public:
		

		//���ܵĹ��캯������
		Json();
		Json(int value);
		Json(double value);
		Json(bool value);
		Json(const std::string& value);
		Json(const char* value);//�ַ�������ֵ����

		//�����Ĺ��캯������JsonProxyʹ��
		Json(std::unique_ptr <json::JsonValue> ptr);


		//�������ƶ�����������
		Json(const Json& other);//��������
		Json& operator=(const Json& other);//������ֵ
		Json(Json&& other) noexcept;//�ƶ����캯��
		Json& operator=(Json&& other) noexcept;//�ƶ���ֵ


		


		~Json();//��������


		//�ӿ�API

		//���л�
		std::string to_string() const;

		//��ȡԭʼ����
		std::string get_string() const;
		//��ȡ����
		json::JsonType type() const;



		//����
		std::unique_ptr<json::JsonValue> clone() const;

		//���������Ԫ��
		void add(const Json& value);

		//�ƶ�
		void add(Json&& value);

		static Json parse(const std::string& content);




		//����Json
		class JsonProxy
		{
		public:

			//���캯��
			JsonProxy(std::unique_ptr<json::JsonValue>& ptr) :ptr_ref(ptr) {}


			//������ֵ
			JsonProxy& operator=(const Json& other)
			{
				ptr_ref = other.clone();
				return *this;
			}

			//�ƶ���ֵ
			JsonProxy& operator=(Json&& other) noexcept 
			{
				ptr_ref = std::move(other.value_ptr);
				return *this;

			}
			
			//��ʽ����ת��JsonProxy->Json
			//����ʵ�֣��ⲿʵ�ֻᱨ��
			operator Json()const {
				return Json(ptr_ref ? ptr_ref->clone() : nullptr);
			}

			//ʵ��data[key].add()
			void add(const Json& value) 
			{
				if (ptr_ref->type() != json::JsonType::ARRAY)
				{
					ptr_ref = std::make_unique<json::JsonArray>();
				}
				auto* arr_ptr = dynamic_cast<json::JsonArray*>(ptr_ref.get());
				if (arr_ptr)
				{
					arr_ptr->add(value.value_ptr->clone());
				}
			}
			void add(Json&& value) 
			{
				if (ptr_ref->type() != json::JsonType::ARRAY)
				{
					ptr_ref = std::make_unique<json::JsonArray>();
				}
				auto* arr_ptr = dynamic_cast<json::JsonArray*>(ptr_ref.get());
				if (arr_ptr)
				{
					arr_ptr->add(std::move(value.value_ptr));
				}
				
			}


			JsonProxy operator[](size_t index)
			{
				if (ptr_ref->type() != json::JsonType::ARRAY)
				{
					ptr_ref = std::make_unique<json::JsonArray>();
				}
				auto* arr_ptr = dynamic_cast<json::JsonArray*>(ptr_ref.get());

				return JsonProxy(arr_ptr->get(index));
			}


			JsonProxy operator [](const std::string& key)
			{
				if (ptr_ref->type() != json::JsonType::OBJECT)
				{
					ptr_ref = std::make_unique<json::JsonObject>();
				}
				auto* obj_ptr = dynamic_cast<json::JsonObject*>(ptr_ref.get());

				return JsonProxy(obj_ptr->get(key));
			}

		private:
			std::unique_ptr<json::JsonValue>& ptr_ref;
		};
		//1. ����������ʵ� operator[]
		JsonProxy operator[](size_t index);

		//2. ���ڶ�����ʵ� operator[]
		JsonProxy operator[](const std::string& key);


	private:
		//Ψһ�ĳ�Ա������ָ���ڲ��������͵�ָ��
		std::unique_ptr <json::JsonValue>value_ptr;


	};



	std::ostream& operator<<(std::ostream& os, const Json& json);

}
