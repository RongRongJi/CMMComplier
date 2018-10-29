#pragma once
#include<string>
#include "token.h"
#include<sstream>
using namespace std;
class parseexception
{
public:
	parseexception(int errNo,token tk);
	~parseexception();
	parseexception(string tokenType, token tk);
	string message();
	void addToErrorMap();
private:
	int m_errNo;
	string m_tokenType;
	token m_token;
};

