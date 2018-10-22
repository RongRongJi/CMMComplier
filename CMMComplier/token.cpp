#include "token.h"
#include<sstream>


token::token()
{
	m_type = NULLL;
	m_lineNo = -1;
	m_columnNo = -1;
	m_value = "";
}

token::token(int type, int lineNo, int columnNo, string value) {
	m_type = type;
	m_lineNo = lineNo;
	m_columnNo = columnNo;
	m_value = value;
}

token::~token()
{
}

//输出token相关信息
//return: 返回此token的类型、行号等信息
string token::toString() {
	stringstream stream;
	stream << m_lineNo << ": ";
	switch (m_type) {
	case ERR:
		stream << "错误: " << m_value;
		return stream.str();
	case IF:
	case ELSE:
	case WHILE:
	case READ:
	case WRITE:
	case INT:
	case REAL:
	case CHAR:
	case VOI:
	case FOR:
	case RETURN:
		stream << "保留字: " << m_value;
		return stream.str();
	case PLUS:
	case MINUS:
	case MUL:
	case DIV:
	case ASSIGN:
	case LT:
	case GT:
	case EQ:
	case NEQ:
	case LPS:
	case RPS:
	case SEMI:
	case LBRACE:
	case RBRACE:
	case LCOM:
	case RCOM:
	case SCOM:
	case LBRACKET:
	case RBRACKET:
	case SINQS:
	case DOUQS:
	case LET:
	case GET:
		stream << m_value;
		return stream.str();
	case ID:
		stream << "标识符, name = " << m_value;
		return stream.str();
	case INT_VALUE:
		stream << "int, value = " << m_value;
		return stream.str();
	case REAL_VALUE:
		stream << "real, value = " << m_value;
		return stream.str();
	case CHAR_VALUE:
		stream << "char, value = " << m_value;
	case STR_VALUE:
		stream << "string, value = " << m_value;
	default:
		return stream.str();
	}
}

//获取行号
//return: 行号
int token::lineNoValue() {
	return m_lineNo;
}

//获取列号
//return: 列号
int token::columnNoValue() {
	return m_columnNo;
}


//获取类型值
//return: 类型值
int token::typeValue() {
	return m_type;
}

//获取类型
//return: 类型
string token::value() const {
	return m_value;
}

//将类型值转换为类型
//parameter:类型值
//return:类型
string token::typeToString(int type) {
	switch (type) {
	case NULLL: return "";
	case IF: return "if";
	case ELSE: return "else";
	case WHILE: return "while";
	case FOR: return "for";
	case READ: return "read";
	case WRITE: return "write";
	case INT: return "int";
	case REAL: return "real";
	case CHAR: return "char";
	case VOI: return "void";
	case RETURN: return "return";
	case PLUS: return "+";
	case MINUS: return "-";
	case MUL: return "*";
	case DIV: return "/";
	case ASSIGN: return "=";
	case LT: return "<";
	case GT: return ">";
	case EQ: return "==";
	case NEQ: return "<>";
	case LPS: return "(";
	case RPS: return ")";
	case SEMI: return ";";
	case LBRACE: return "{";
	case RBRACE: return "}";
	case LCOM: return "/*";
	case RCOM: return "*/";
	case SCOM: return "//";
	case LBRACKET: return "[";
	case RBRACKET: return "]";
	case SINQS: return "'";
	case DOUQS: return "\"";
	case LET: return "<=";
	case GET: return ">=";
	case ID: return "id";
	case INT_VALUE: return "int value";
	case REAL_VALUE: return "real value";
	case CHAR_VALUE: return "char value";
	case STR_VALUE: return "string value";
	default: return "UNKNOWN";
	}
}