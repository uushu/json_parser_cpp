#ifndef JSON_H
#define JSON_H


#include<string>
#include<vector> //array
#include<memory> //unique_ptr
#include<map> //object




namespace json
{

	//Jsonֵ����
	//ʹ��enum class��֤���Ͱ�ȫ
	enum class JsonType
	{
		NUL,
		BOOL,
		NUMBER,
		STRING,
		ARRAY,
		OBJECT,

	};


	//�������,��������ͨ�ýӿ�
	class JsonValue
	{
	public:
		//������������ȷ��ͨ������ָ��ɾ�����������ʱ����ȷ�������������������

		virtual ~JsonValue() = default;

		//���麯������ȡֵ������
		virtual JsonType type() const = 0;

		//���л���������ת�����ַ�����ʽ
		virtual std::string to_string() const = 0;

		//ʵ���������������������ָ�򸱱���ָ��
		virtual std::unique_ptr<JsonValue>clone()const = 0;



	};

	//Json����������

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
		//��ʾ���죬��ֹ��������bool���͵�JsonBool���͵���ʽת��
		//Ҳ���ǲ���д�� JsonBool b=true;
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
		//���캯����ֵ���ã���ֵ�ƶ����⿽��
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

		//���Ԫ�ص�����
		void add(std::unique_ptr<JsonValue> value);
		
		//��ȡԪ���±�
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
		//��Ӽ�ֵ�Ե�����
		void set(const std::string& key, std::unique_ptr<JsonValue> value);
		//��ȡ����Ӧ��ֵ
		std::unique_ptr<JsonValue>& get(const std::string& key);
	private:
		std::map<std::string, std::unique_ptr<JsonValue>>values;
	};
}

#endif

