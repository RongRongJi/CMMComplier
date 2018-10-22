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

//���token�����Ϣ
//return: ���ش�token�����͡��кŵ���Ϣ
string token::toString() {
	stringstream stream;
	stream << m_lineNo << ": ";
	switch (m_type) {
	case ERR:
		stream << "����: " << m_value;
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
		stream << "������: " << m_value;
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
		stream << "��ʶ��, name = " << m_value;
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

//��ȡ�к�
//return: �к�
int token::lineNoValue() {
	return m_lineNo;
}

//��ȡ�к�
//return: �к�
int token::columnNoValue() {
	return m_columnNo;
}


//��ȡ����ֵ
//return: ����ֵ
int token::typeValue() {
	return m_type;
}

//��ȡ����
//return: ����
string token::value() const {
	return m_value;
}

//������ֵת��Ϊ����
//parameter:����ֵ
//return:����
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