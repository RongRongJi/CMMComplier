#pragma once
#include<string>
using namespace std;
class token
{
public:
	//符号表
	enum {
		//空
		NULLL,
		//错误标记
		ERR,
		//保留字
		IF,
		ELSE,
		WHILE,
		FOR,
		READ,
		WRITE,
		INT,
		REAL,
		CHAR,
		VOI,
		//特殊符号
		PLUS,//+
		MINUS,//-
		MUL,//*
		DIV,// /
		ASSIGN,// =
		LT,// <
		GT,// >
		EQ,// ==
		NEQ,// <>
		LPS,// (
		RPS,// )
		SEMI,// ;
		LBRACE,//{
		RBRACE,// }
		LCOM,// /*
		RCOM,// */
		SCOM,// //
		LBRACKET,// [
		RBRACKET,// ]
		SINQS, // \'
		DOUQS, // \"
		LET,// <=
		GET,// >=
		//标识符
		ID,
		RETURN,
		//十进制的整数与实数
		INT_VALUE,
		REAL_VALUE,
		CHAR_VALUE,
		STR_VALUE

	};
	token();
	token(int type, int lineNo, int columnNo, string value = "");
	virtual ~token();
	string toString();				//将token相关信息输出
	int lineNoValue();				//行号
	int columnNoValue();			//列号
	int typeValue();				//token类型编号
	string value() const;			//token类型
	static string typeToString(int type);	//token类型编号翻译成类型
private:
	int m_type;				//类型号
	int m_lineNo;			//行号
	int m_columnNo;			//列号
	string m_value;			//类型
};

