#pragma once
#include<string>
using namespace std;
class codegeneraterException
{
public:
	~codegeneraterException();
	codegeneraterException(int lineNo, string errorStr);
	string message();

private:
	int m_lineNo;
	string m_errorStr;
};

