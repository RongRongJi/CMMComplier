#include "value.h"
#include <stdlib.h>
#include<sstream>
#include <cstring>
#include<string>
#include"common.h"
using namespace std;

int value::addrUnitSize = 1;
int value::callfrom = -10;

value::value()
{
}


value::~value()
{
}

value::value(string  valueStr, int type):m_type(type)
{
	bool isChar = false;
	if (valueStr[0] == '\'')
		isChar = true;
	switch (type) {
	case INT_VALUE:
		m_size = 4;
		if (isChar)
			m_valueStr = valueStr[1];
		else
			m_valueStr = itos(atoi(valueStr.c_str()));
		break;
	case REAL_VALUE:
		m_size = 8;
		if (isChar)
			m_valueStr = valueStr[1];
		else
			m_valueStr = dtos(atof(valueStr.c_str()));
		break;
	case CHAR_VALUE:
		m_size = 1;
		if (isChar)
			m_valueStr = valueStr;
		else
			m_valueStr = "'" + ctos(char(atoi(valueStr.c_str()))) + "'";
		break;
	case ADDR:
		m_size = 4;
		m_valueStr = valueStr;
		break;
	default:
		break;
	}
}

string value::valueStr()
{
	return m_valueStr;
}

void value::setValueStr(string & valueStr)
{
	m_valueStr = valueStr;
}

int value::type()
{
	return m_type;
}

void value::setType(int type)
{
	m_type = type;
}

int value::size()
{
	return m_size;
}

void value::setSize(int size)
{
	m_size = size;
}

string value::typeToStr()
{
	switch (m_type) {
	case INT_VALUE:
		return "int";
	case REAL_VALUE:
		return "real";
	case CHAR_VALUE:
		return "char";
	case ADDR:
		return "address";
	default:
		return "";
	}
}

int toInt(value v)
{
	switch (v.m_type) {
	case value::CHAR_VALUE:
		return v.m_valueStr[1];
	case value::INT_VALUE:
		return atoi(v.m_valueStr.c_str());
	case value::REAL_VALUE:
		return 0;
	case value::ADDR:
		return atoi(v.m_valueStr.substr(1, v.m_valueStr.size() - 1).c_str());
	}
	return 0;
}

double toDouble(value v)
{
	switch (v.m_type) {
	case value::CHAR_VALUE:
		return v.m_valueStr[1];
	case value::INT_VALUE:
	case value::REAL_VALUE:
		return atof(v.m_valueStr.c_str());
	}
	return 0;
}

string nequal(value  l_value, value  r_value)
{
	int result = 0;
	if ((l_value == r_value) == "0")
		result = 1;
	return itos(result);
}

std::string operator+(value  l_value, value  r_value)
{
	if ((l_value.type() == value::INT_VALUE || l_value.type() == value::CHAR_VALUE)
		&& (r_value.type() == value::INT_VALUE || r_value.type() == value::CHAR_VALUE)) {
		int result = toInt(l_value) + toInt(r_value);
		return itos(result);
	}
	else if (l_value.type() == value::REAL_VALUE || r_value.type() == value::REAL_VALUE) {
		double result = toDouble(l_value) + toDouble(r_value);
		return dtos(result);
	}
	return "0";
}

std::string operator-(value  l_value, value  r_value)
{
	if ((l_value.type() == value::INT_VALUE || l_value.type() == value::CHAR_VALUE)
		&& (r_value.type() == value::INT_VALUE || r_value.type() == value::CHAR_VALUE)) {
		int result = toInt(l_value) - toInt(r_value);
		return itos(result);
	}
	else if (l_value.type() == value::REAL_VALUE || r_value.type() == value::REAL_VALUE) {
		double result = toDouble(l_value) - toDouble(r_value);
		return dtos(result);
	}
	return "0";
}

std::string operator*(value  l_value, value  r_value)
{
	if ((l_value.type() == value::INT_VALUE || l_value.type() == value::CHAR_VALUE)
		&& (r_value.type() == value::INT_VALUE || r_value.type() == value::CHAR_VALUE)) {
		int result = toInt(l_value) * toInt(r_value);
		return itos(result);
	}
	else if (l_value.type() == value::REAL_VALUE || r_value.type() == value::REAL_VALUE) {
		double result = toDouble(l_value) * toDouble(r_value);
		return dtos(result);
	}
	return "0";
}

std::string operator/(value  l_value, value  r_value)
{
	if ((l_value.type() == value::INT_VALUE || l_value.type() == value::CHAR_VALUE)
		&& (r_value.type() == value::INT_VALUE || r_value.type() == value::CHAR_VALUE)) {
		if (toInt(r_value) == 0) {
			if (value::callfrom == -10) {
				return "除数不能为0";
			}
			else {
				int tempLineNo = value::callfrom;
				value::callfrom = -10;
				return "除数不能为0";
			}
		}
		int result = toInt(l_value) / toInt(r_value);
		return itos(result);
	}
	else if (l_value.type() == value::REAL_VALUE || r_value.type() == value::REAL_VALUE) {
		double result = toDouble(l_value) + toDouble(r_value);
		return dtos(result);
	}
	return "0";
}

std::string operator>(value  l_value, value  r_value)
{
	int result = 0;
	result = (atof((l_value - r_value).c_str()) > 0);
	return itos(result);
}

std::string operator>=(value  l_value, value  r_value)
{
	int result = 0;
	result = (atof((l_value - r_value).c_str()) >= 0);
	return itos(result);
}

std::string operator<(value l_value,value  r_value)
{
	int result = 0;
	if (r_value > l_value != "0")
		result = 1;
	return itos(result);
}

std::string operator<=( value  l_value, value  r_value)
{
	int result = 0;
	if (r_value >= l_value != "0")
		result = 1;
	return itos(result);
}

std::string operator==(value  l_value, value  r_value)
{
	int result = 0;
	if (l_value.type() == value::STR_VALUE && r_value.type() == value::STR_VALUE)
		result = (strcmp(l_value.valueStr().c_str(), r_value.valueStr().c_str()) == 0);
	else
		result = (atof((l_value - r_value).c_str()) == 0);
	return itos(result);
}
