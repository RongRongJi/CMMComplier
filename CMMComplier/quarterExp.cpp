#include "quarterExp.h"
#include <sstream>
#include "common.h"

int quarterExp::m_index = -1;
 string quarterExp::JMP = "jmp";
 string quarterExp::READ = "read";
 string quarterExp::WRITE = "write";
 string quarterExp::IN = "in";
 string quarterExp::OUT = "out";
 string quarterExp::INT = "int";
 string quarterExp::REAL = "real";
 string quarterExp::CHAR = "char";
 string quarterExp::ASSIGN = "assign";
 string quarterExp::PLUS = "+";
 string quarterExp::MINUS = "-";
 string quarterExp::MUL = "*";
 string quarterExp::DIV = "/";
 string quarterExp::GT = ">";
 string quarterExp::LT = "<";
 string quarterExp::GET = ">=";
 string quarterExp::LET = "<=";
 string quarterExp::EQ = "==";
 string quarterExp::NEQ = "<>";
 string quarterExp::CALL = "call";
 string quarterExp::CALLFH = "callfh";
 string quarterExp::VOID = "void";

quarterExp::quarterExp()
{
}


quarterExp::~quarterExp()
{
}

quarterExp::quarterExp(string first, string second, string third, string fourth, int lineNo)
{
	m_first = first;
	m_second = second;
	m_third = third;
	m_fourth = fourth;
	m_lineNo = lineNo;
	m_index++;
}

string quarterExp::first()
{
	return m_first;
}

void quarterExp::setFirst(string  first)
{
	m_first = first;
}

string quarterExp::second()
{
	return m_second;
}

void quarterExp::setSecond(string  second)
{
	m_second = second;
}

string quarterExp::third()
{
	return m_third;
}

void quarterExp::setThird(string  third)
{
	m_third = third;
}

string quarterExp::fourth()
{
	return m_fourth;
}

void quarterExp::setFourth(string fourth)
{
	m_fourth = fourth;
}

int quarterExp::index()
{
	return m_index;
}

void quarterExp::setIndex(int index)
{
	m_index = index;
}

int quarterExp::lineNo()
{
	return m_lineNo;
}

void quarterExp::setLineNo(int lineNo)
{
	m_lineNo = lineNo;
}

string quarterExp::toString()
{
	return "ÐÐºÅ " + itos(m_lineNo) + ": (" + m_first + ", " + m_second + ", " + m_third + ", " + m_fourth + ")";
}

