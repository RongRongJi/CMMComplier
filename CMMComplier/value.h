#pragma once
#include<string>
#include<sstream>
#include<iostream>
using namespace std;
class value
{
public:
	enum {
		INT_VALUE,
		REAL_VALUE,
		CHAR_VALUE,
		STR_VALUE,
		SINGLE_VALUE,
		VOID,
		ADDR
	};
	value();
	~value();

	value(string  valueStr, int type);
	static int addrUnitSize;
	static int callfrom;
	string valueStr();
	void setValueStr(string & valueStr);
	int type();
	void setType(int type);
	int size();
	void setSize(int size);
	string typeToStr();
	friend int toInt(value v);
	friend double toDouble(value v);
	friend string nequal(value  l_value, value  r_value);

	/*
	* 重载运算符
	*/
	friend std::string operator+(value  l_value,  value  r_value);
	friend std::string operator-( value  l_value,  value  r_value);
	friend std::string operator*( value  l_value,  value  r_value);
	friend std::string operator/( value  l_value,  value  r_value);
	friend std::string operator>( value  l_value,  value  r_value);
	friend std::string operator>=( value  l_value,  value  r_value);
	friend std::string operator<( value  l_value,  value  r_value);
	friend std::string operator<=( value  l_value,  value  r_value);
	friend std::string operator==( value  l_value,  value  r_value);

private:
	string m_valueStr;		//数值
	int m_type;				//数值类型
	int m_size;				//数值大小
};

