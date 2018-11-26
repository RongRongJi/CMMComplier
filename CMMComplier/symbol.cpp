#include "symbol.h"



symbol::symbol()
{
}


symbol::~symbol()
{
}

symbol::symbol(const string & name, int type):m_name(name),m_type(type)
{
}

symbol::symbol(int valueIndex, int type):m_valueIndex(valueIndex),m_type(type)
{
}

symbol::symbol(const string & name, int type, int lineNo, int level, int elementNum)
	: m_name(name), m_type(type), m_lineNo(lineNo), m_level(level), m_elementNum(elementNum)
{
	switch (type) {
	case SINGLE_INT:
		m_dataSize = 4;
		break;
	case SINGLE_REAL:
		m_dataSize = 8;
		break;
	case SINGLE_CHAR:
		m_dataSize = 1;
		break;
	}
}

int symbol::type()
{
	return m_type;
}

void symbol::setType(int type)
{
	m_type = type;
}

int symbol::dataSize()
{
	return m_dataSize;
}

void symbol::setDataSize(int dataSize)
{
	m_dataSize = dataSize;
}

int symbol::level()
{
	return m_level;
}

void symbol::setLeve(int level)
{
	m_level = level;
}

string symbol::name()
{
	return m_name;
}

void symbol::setName(string & name)
{
	m_name = name;
}

int symbol::elementNum()
{
	return m_elementNum;
}

void symbol::setElementNum(int elementNum)
{
	m_elementNum = elementNum;
}

int symbol::lineNo()
{
	return m_lineNo;
}

void symbol::setLineNo(int lineNo)
{
	m_lineNo = lineNo;
}

int symbol::valueIndex()
{
	return m_valueIndex;
}

void symbol::setValueIndex(int valueIndex)
{
	m_valueIndex = valueIndex;
}
