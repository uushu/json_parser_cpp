#pragma once

#include<string>


namespace json
{
	class Json;
	class Parser
	{
	public:
		//������Ҫ������JSON�ַ���
		Parser(const std::string& c);

		//����JSON�ַ���
		Json parse();

	private:
		//����JSNN�ַ��ĸ�������
		//�������пո񡢻��С��Ʊ���ȿհ��ַ�
		void skip_whitespace();
		//��ȡ��ǰλ�õ��ַ�
		char peek() const;

		//��ȡ��ǰ�ַ���ǰ������һ���ַ�
		char get_advance();

		//���������͵ĺ���

		Json parse_null();
		Json parse_bool();
		Json parse_string();
		Json parse_number();
		Json parse_array();
		Json parse_object();

		//���ݵ�ǰ�ַ����������ĸ���������
		Json parse_value();

	private:
		std::string content;//��������JSON�ַ���
		size_t index;//��ǰ����λ��
	};
}