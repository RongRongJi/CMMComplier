#pragma once
#include<vector>
#include<string>
#include<sstream>
#include "token.h"
#include "error.h"
using namespace std;
class lexer
{
public:
	lexer();
	vector<token> LexAnalyze(const string & text);//词法分析
	~lexer();
private:
	void readChar();		//读取一个字符
	bool isBlank();			//判断是否为空白
	bool isSingle();		//判断是否是单个符号
	bool isCombine();		//判断是否为复合符号
	bool isDigit();			//判断是否为数字
	bool isReserveWordOrID();	//判断是否是保留字或标识符
	void errorHandle(stringstream& stream);		//错误检测

	bool m_isEOF;			//是否读取结束
	int m_lineNo;			//行号
	int m_columnNo;			//列号
	vector<token> m_tokenVec;	//存放token
	char m_currentChar;		//当前读取的字符
	stringstream m_stream;	//传入流
};

