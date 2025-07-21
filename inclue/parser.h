#pragma once

#include<string>


namespace json
{
	class Json;
	class Parser
	{
	public:
		//接受需要解析的JSON字符串
		Parser(const std::string& c);

		//解析JSON字符串
		Json parse();

	private:
		//解析JSNN字符的辅助函数
		//跳过所有空格、换行、制表符等空白字符
		void skip_whitespace();
		//获取当前位置的字符
		char peek() const;

		//获取当前字符并前进到下一个字符
		char get_advance();

		//解析各类型的函数

		Json parse_null();
		Json parse_bool();
		Json parse_string();
		Json parse_number();
		Json parse_array();
		Json parse_object();

		//根据当前字符决定调用哪个解析函数
		Json parse_value();

	private:
		std::string content;//待解析的JSON字符串
		size_t index;//当前解析位置
	};
}
