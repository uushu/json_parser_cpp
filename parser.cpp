

#include"jsonvalue.h"
#include"json.h"

#include"parser.h"

#include<stdexcept>


json::Parser::Parser(const std::string& c):content(c),index(0){}

json::Json json::Parser::parse()
{
	skip_whitespace();
	auto result = parse_value();//�����ɺ���
	skip_whitespace();
	if (index != content.size())
	{
		throw std::logic_error("Extra characters at end of JSON string");
	}
	return result;
}

void json::Parser::skip_whitespace()
{
	while (index < content.size() && std::isspace(content[index]))
	{
		index++;

	}

}




char json::Parser::peek() const
{
	if (index >= content.size())
	{
		throw std::logic_error("Unexcepted end of input");
	}
	return content[index];// ���ص�ǰλ�õ��ַ��������ƶ�λ��
		
}


char json::Parser::get_advance()
{
	if (index >= content.size())
	{
		throw std::logic_error("Unexcepted end of input");

	}
	return content[index++];// ���ص�ǰ�ַ���Ȼ��index+1

}

json::Json json::Parser::parse_null()
{
	if (content.substr(index, 4) == "null")
	{
		index += 4;
		return Json();
	}
	throw std::logic_error("Failed to parser null");
}

json::Json json :: Parser::parse_bool()
{
	if (content.substr(index, 4) == "true")
	{
		index += 4;
		return Json(true);
	}
	else if (content.substr(index, 5) == "false")
	{
		index += 5;
		return Json(false);
	}
	throw std::logic_error("Failed to parser bool");


}

json::Json json::Parser::parse_string()
{
	std::string result;
	if (get_advance() != '"') //��鿪ͷ������
	{
		throw std::logic_error("String must start with a double quote");
	}
	while (index < content.size())
	{
		char ch = get_advance();
		if (ch == '"') //������������
		{
			return Json(result);
		}
		else if (ch == '\\') //����ת���ַ�
		{
			ch = get_advance();
			switch (ch)
			{
			case 'n':
				result += '\n';
				break;
			case 't':
				result += '\t';
				break;
			case 'r':
				result += '\r';
				break;
			case 'b':
				result += '\b';
				break;
			case 'f':
				result += '\f';
				break;
			case '"':
			case '\\':
			case '/':
				result += ch;
				break;
			default:
				throw std::logic_error("Invalid escape sequence in string");
			}
		}
		else
		{
			result += ch; //��ͨ�ַ�ֱ�����
		}
	}
	throw std::logic_error("Unexcepted end of input in string");
}

json::Json json::Parser::parse_number()
{
	size_t start = index;


	while (index < content.size() && (std::isdigit(content[index]) ||
		content[index] == '.' ||
		content[index] == '-' ||
		content[index] == 'e' ||
		content[index] == 'E' ||
		content[index] == '+'))
	{
		index++;
	}

	std::string num_str = content.substr(start, index - start);

	if (num_str.empty())
	{
		throw std::logic_error("Failed to parsre number");

	}

	try
	{
		double num = std::stod(num_str);
		return Json(num);

	}
	catch (const std::invalid_argument&)
	{
		throw std::logic_error("Invalid number format: ");
	}
	catch (const std::out_of_range&)
	{
		throw std::logic_error("Number out of range");
	}

}
json::Json json::Parser::parse_array()
{
	if (get_advance() != '[') //��鿪ͷ�ķ�����
	{
		throw std::logic_error("Array must start with a left bracket");
	}
	Json array = Json(std::make_unique<json::JsonArray>());
	skip_whitespace();
	if (peek() == ']') //���������
	{
		get_advance(); //����������
		return array;
	}
	while (true)
	{
		array.add(parse_value()); //��������Ԫ��
		skip_whitespace();
		char ch = get_advance();
		if (ch == ']') //���������Ž���
		{
			return array;
		}
		else if (ch != ',') //���ŷָ���
		{
			throw std::logic_error("Expected ',' or ']' in array");
		}

		skip_whitespace();
	}
}

json::Json json::Parser::parse_object()
{
	if (get_advance() != '{') { 
		throw std::logic_error("Object must start with a left brace");
	}

	Json object = Json(std::make_unique<json::JsonObject>());
	skip_whitespace();

	if (peek() == '}') {
		get_advance(); 
		return object;
	}

	while (true) {
		// 1. Parse the key (which must be a string)
		Json key_json = parse_string();
		std::string key = key_json.get_string(); // Helper function needed

		skip_whitespace();

		
		if (get_advance() != ':') {
			throw std::logic_error("Expected ':' after key in object");
		}
		skip_whitespace();

		
		object[key] = parse_value();

		skip_whitespace();
		char ch = get_advance();

		if (ch == '}') { 
			return object;
		}
		if (ch != ',') { 
			throw std::logic_error("Expected ',' or '}' in object");
		}
		skip_whitespace();
	}
}





json::Json json::Parser::parse_value()
{
	char ch = peek();
	switch (ch) {
	case 'n':
		return parse_null();
	case 't':
	case 'f':
		return parse_bool();
	case '"':
		return parse_string();
	case '[':
		return parse_array();
	case '{':
		return parse_object();
	case '-':
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		return parse_number();
	default:
		throw std::logic_error("Invalid starting character for a JSON value");
	}
}


