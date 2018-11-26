#pragma once
#include<string>
using namespace std;
class executeException
{
public:
	~executeException();
	executeException(int lineNo, string  errorStr);
	string message();
private:
	int m_lineNo;
	string m_errorStr;
};

