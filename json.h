#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "jsonvalue.h"

#include"parser.h"

namespace json
{

	//构建一个用户可以直接使用的万能json类
	class Json
	{

	public:
		

		//可能的构造函数类型
		Json();
		Json(int value);
		Json(double value);
		Json(bool value);
		Json(const std::string& value);
		Json(const char* value);//字符串字面值常量

		//新增的构造函数，供JsonProxy使用
		Json(std::unique_ptr <json::JsonValue> ptr);


		//拷贝、移动、析构函数
		Json(const Json& other);//拷贝构造
		Json& operator=(const Json& other);//拷贝赋值
		Json(Json&& other) noexcept;//移动构造函数
		Json& operator=(Json&& other) noexcept;//移动赋值


		


		~Json();//析构函数


		//接口API

		//序列化
		std::string to_string() const;

		//获取原始数据
		std::string get_string() const;
		//获取类型
		json::JsonType type() const;



		//拷贝
		std::unique_ptr<json::JsonValue> clone() const;

		//向数组添加元素
		void add(const Json& value);

		//移动
		void add(Json&& value);

		static Json parse(const std::string& content);




		//代理Json
		class JsonProxy
		{
		public:

			//构造函数
			JsonProxy(std::unique_ptr<json::JsonValue>& ptr) :ptr_ref(ptr) {}


			//拷贝赋值
			JsonProxy& operator=(const Json& other)
			{
				ptr_ref = other.clone();
				return *this;
			}

			//移动赋值
			JsonProxy& operator=(Json&& other) noexcept 
			{
				ptr_ref = std::move(other.value_ptr);
				return *this;

			}
			
			//隐式类型转换JsonProxy->Json
			//内联实现，外部实现会报错
			operator Json()const {
				return Json(ptr_ref ? ptr_ref->clone() : nullptr);
			}

			//实现data[key].add()
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
		//1. 用于数组访问的 operator[]
		JsonProxy operator[](size_t index);

		//2. 用于对象访问的 operator[]
		JsonProxy operator[](const std::string& key);


	private:
		//唯一的成员变量，指向内部数据类型的指针
		std::unique_ptr <json::JsonValue>value_ptr;


	};



	std::ostream& operator<<(std::ostream& os, const Json& json);

}
