#include "executeException.h"
#include<sstream>



executeException::~executeException()
{
}

executeException::executeException(int lineNo, string  errorStr) {
	m_lineNo = lineNo;
	m_errorStr = errorStr;
}

string executeException::message() {
	stringstream ss;
	ss << "ÐÐºÅ " << m_lineNo << ": " << m_errorStr;
	return ss.str();
}
