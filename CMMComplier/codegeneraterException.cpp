#include "codegeneraterException.h"
#include "common.h"
#include <sstream>





codegeneraterException::~codegeneraterException()
{
}

codegeneraterException::codegeneraterException(int lineNo, string  errorStr) {
	m_lineNo = lineNo;
	m_errorStr = errorStr;
}

string codegeneraterException::message() {
	stringstream ss;
	ss << "ÐÐºÅ " << m_lineNo << ": " << m_errorStr;
	return ss.str();
}

