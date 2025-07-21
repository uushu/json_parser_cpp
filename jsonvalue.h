#ifndef JSON_H
#define JSON_H


#include<string>
#include<vector> //array
#include<memory> //unique_ptr
#include<map> //object




namespace json
{

	//Json值类型
	//使用enum class保证类型安全
	enum class JsonType
	{
		NUL,
		BOOL,
		NUMBER,
		STRING,
		ARRAY,
		OBJECT,

	};


	//抽象基类,定义所有通用接口
	class JsonValue
	{
	public:
		//虚析构函数，确保通过基类指针删除派生类对象时能正确调用派生类的析构函数

		virtual ~JsonValue() = default;

		//纯虚函数，获取值的类型
		virtual JsonType type() const = 0;

		//序列化：将对象转换成字符串形式
		virtual std::string to_string() const = 0;

		//实现深拷贝，创建副本，返回指向副本的指针
		virtual std::unique_ptr<JsonValue>clone()const = 0;



	};

	//Json具体派生类

	class JsonNull :public JsonValue
	{
	public:
		JsonType type()const override;

		std::string to_string()const override;

		std::unique_ptr <JsonValue> clone() const override;
	};

	class JsonBool :public JsonValue
	{
	public:
		//显示构造，防止编译器对bool类型到JsonBool类型的隐式转换
		//也就是不能写成 JsonBool b=true;
		explicit JsonBool(bool val) :value(val) {}
		JsonType type() const override;

		std::string to_string() const override;
		std::unique_ptr<JsonValue> clone() const override;

		bool get_bool() const
		{
			return value;
		}

	private:
		bool value;
	};

	class JsonString :public JsonValue
	{
	public:
		//构造函数左值引用，右值移动，免拷贝
		explicit JsonString(const std::string& val) :value(val) {}
		explicit JsonString(std::string&& val) :value(std::move(val)) {}
		JsonType type()const override;
		std::string to_string()const override;
		const std::string& get_string() const;
		std::unique_ptr<JsonValue> clone() const override;


	private:
		std::string value;
	};

	class JsonNumber :public JsonValue
	{
	public:
		explicit JsonNumber(double val) :value(val) {}
		JsonType type()const override;
		std::string to_string()const override;
		std::unique_ptr<JsonValue> clone() const override;
		double get_number() const
		{
			return value;
		}

	private:
		double value;

	};

	class JsonArray :public JsonValue
	{
	public:

		JsonType type() const override;

		std::string to_string() const override;
		std::unique_ptr<JsonValue> clone() const override;

		//添加元素到数组
		void add(std::unique_ptr<JsonValue> value);
		
		//获取元素下标
		std::unique_ptr<JsonValue>& get(size_t index);
		

	private:
		std::vector<std::unique_ptr<JsonValue>> values;

	};


	class JsonObject :public JsonValue
	{
	public:
		JsonType type() const override;
		std::string to_string() const override;
		std::unique_ptr<JsonValue> clone() const override;
		//添加键值对到对象
		void set(const std::string& key, std::unique_ptr<JsonValue> value);
		//获取键对应的值
		std::unique_ptr<JsonValue>& get(const std::string& key);
	private:
		std::map<std::string, std::unique_ptr<JsonValue>>values;
	};
}

#endif

