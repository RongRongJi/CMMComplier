#include "funsymbol.h"



funsymbol::funsymbol()
{
}


funsymbol::~funsymbol()
{
}

funsymbol::funsymbol(const std::string &name, int lineNo, int returnType, int stmtIndex)
{
	m_name = name;
	m_lineNo = lineNo;
	m_returnType = returnType;
	m_stmtIndex = stmtIndex;
}

std::string funsymbol::name() 
{
	return m_name;
}

void funsymbol::setName(const std::string &name)
{
	m_name = name;
}
int funsymbol::lineNo() 
{
	return m_lineNo;
}

void funsymbol::setLineNo(int lineNo)
{
	m_lineNo = lineNo;
}
int funsymbol::returnType() 
{
	return m_returnType;
}

void funsymbol::setReturnType(int returnType)
{
	m_returnType = returnType;
}
int funsymbol::stmtIndex() 
{
	return m_stmtIndex;
}

void funsymbol::setStmtIndex(int stmtIndex)
{
	m_stmtIndex = stmtIndex;
}