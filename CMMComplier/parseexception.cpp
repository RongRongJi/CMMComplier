#include "parseexception.h"



parseexception::parseexception(int errNo,token tk):m_errNo(errNo),m_token(tk),m_tokenType("")
{
}


parseexception::~parseexception()
{
}

parseexception::parseexception(string tokenType, token tk):m_errNo(-1),m_tokenType(tokenType),m_token(tk)
{
}

string parseexception::message()
{
	stringstream tokenStream;
	if (m_errNo == -1)
		tokenStream << "line " << m_token.lineNoValue() << ": \"" << m_token.value() << "\", expected \"" << m_tokenType << "\"";
	else
		tokenStream << "line " << m_token.lineNoValue() << ": \"" << m_token.value() << "\", expected \"" << token::typeToString(m_errNo) << "\"";
	return tokenStream.str();
}

void parseexception::addToErrorMap()
{
	stringstream stream;
	if (m_errNo == -1)
		stream << "expected '" << m_tokenType << "', got '" << m_token.value() << "'";
	else
		stream << "expected '" << token::typeToString(m_errNo) << "', got '" << m_token.value() << "'";
	//errorMap.insert(make_pair(m_token.lineNoValue(), stream.str()));
}
