#include "lexer.h"



lexer::lexer()
{
	m_isEOF = false;
	m_lineNo = 1;
	m_columnNo = 0;
	m_currentChar = ' ';
}


lexer::~lexer()
{
}

//词法分析
//parameter: 需要进行词法分析的文段text
//return: 该文段所有的词法分析结果
vector<token> lexer::LexAnalyze(const string& text) {
	m_tokenVec.clear();
	m_isEOF = false;
	//m_lineNo = 1;
	m_stream.clear();
	m_stream.str(text);

	readChar();
	while (!m_isEOF) {
		if (isDigit()) {
			continue;
		}
		if (isBlank()) {
			readChar();
			continue;
		}
		if(isSingle()) {
			readChar();
			continue;
		}
		if (isCombine()) {
			continue;
		}
		if (isReserveWordOrID()) {
			continue;
		}
		errorVec.push_back(2);
		stringstream stream;
		stream<<m_currentChar <<"(行号: " << m_lineNo << " 列号: " << m_columnNo << ")";
		m_tokenVec.push_back(token(token::ERR, m_lineNo, m_columnNo, errorStr(2)+stream.str()));

		readChar();
		
	}
	return m_tokenVec;
}

//词法分析错误处理
//parameter: 写入错误字段所需的流
void lexer::errorHandle(stringstream& stream) {
	bool isError = true;
	while (isError && !m_isEOF) {
		if (isBlank()) {
			readChar();
			isError = false;
			continue;
		}
		if (isSingle()) {
			readChar();
			isError = false;
			continue;
		}
		if (isCombine()) {
			readChar();
			isError = false;
			continue;
		}
		stream << m_currentChar;
		readChar();
		if (m_currentChar == ';')
			break;
	}
}

//向后读取一个字符
void lexer::readChar() {
	m_currentChar = m_stream.get();
	m_columnNo++;
	if (int(m_currentChar) == -1) {
		m_isEOF = true;
	}
}

//字符是否为空格、tab键、回车换行等分隔符
//return: 是返回true，否返回false
bool lexer::isBlank() {
	if (m_currentChar==' '||m_currentChar==10||m_currentChar==9) {
		if (m_currentChar == '\n') {
			m_lineNo++;
			m_columnNo = 0;
		}
		return true;
	}
	else {
		return false;
	}
}

//是否为单字符符号
//return: 是返回true，否（/，<，>，=包括在内）返回false
bool lexer::isSingle() {
	switch (m_currentChar) {
	case '+':
		m_tokenVec.push_back(token(token::PLUS, m_lineNo, m_columnNo, "+"));
		return true;
	case '-':
		m_tokenVec.push_back(token(token::MINUS, m_lineNo, m_columnNo, "-"));
		return true;
	case '*':
		m_tokenVec.push_back(token(token::MUL, m_lineNo, m_columnNo, "*"));
		return true;
	case ';':
		m_tokenVec.push_back(token(token::SEMI, m_lineNo, m_columnNo, ";"));
		return true;
	case '(':
		m_tokenVec.push_back(token(token::LPS, m_lineNo, m_columnNo, "("));
		return true;
	case ')':
		m_tokenVec.push_back(token(token::RPS, m_lineNo, m_columnNo, ")"));
		return true;
	case '[':
		m_tokenVec.push_back(token(token::LBRACKET, m_lineNo, m_columnNo, "["));
		return true;
	case ']':
		m_tokenVec.push_back(token(token::RBRACKET, m_lineNo, m_columnNo, "]"));
		return true;
	case '{':
		m_tokenVec.push_back(token(token::LBRACE, m_lineNo, m_columnNo, "{"));
		return true;
	case '}':
		m_tokenVec.push_back(token(token::RBRACE, m_lineNo, m_columnNo, "}"));
		return true;
	default:
		return false;
	}
	
}

//是否为多字符组合的复合符号
//return: 是（/，<，>，=包括在内）返回true，否返回false
bool lexer::isCombine() {
	if (m_currentChar == '/') {
		readChar();
		if (m_currentChar == '*') {
			//多行注释
			m_tokenVec.push_back(token(token::LCOM, m_lineNo, m_columnNo, "/*"));
			readChar();
			while (!m_isEOF) {
				if (m_currentChar == '\n')
					m_lineNo++;
				if (m_currentChar == '*') {
					readChar();
					if (m_currentChar == '/') {
						m_tokenVec.push_back(token(token::RCOM, m_lineNo, m_columnNo, "*/"));
						break;
					}
				}
				else {
					readChar();
				}
			}if (!m_isEOF)
				readChar();
		}
		else if (m_currentChar == '/') {
			//单行注释
			m_tokenVec.push_back(token(token::SCOM, m_lineNo, m_columnNo, "//"));
			readChar();
			while (!m_isEOF && m_currentChar != '\n')
				readChar();
			if (m_currentChar == '\n') {
				m_lineNo++;
				readChar();
			}
		}
		else {
			//除号
			m_tokenVec.push_back(token(token::DIV, m_lineNo, m_columnNo, "/"));
		}
		return true;
	}
	else if (m_currentChar == '=') {
		readChar();
		if (m_currentChar == '=') {
			m_tokenVec.push_back(token(token::EQ, m_lineNo, m_columnNo, "=="));
			readChar();
		}
		else {
			m_tokenVec.push_back(token(token::ASSIGN, m_lineNo, m_columnNo, "="));
		}
		return true;
	}
	else if (m_currentChar == '>') {
		readChar();
		if (m_currentChar == '=') {
			m_tokenVec.push_back(token(token::GET, m_lineNo, m_columnNo, ">="));
			readChar();
		}
		else {
			m_tokenVec.push_back(token(token::GT, m_lineNo, m_columnNo, ">"));
		}
		return true;
	}
	else if (m_currentChar == '<') {
		readChar();
		if (m_currentChar == '=') {
			m_tokenVec.push_back(token(token::LET, m_lineNo, m_columnNo, "<="));
			readChar();
		}
		else if (m_currentChar == '>') {
			m_tokenVec.push_back(token(token::NEQ, m_lineNo, m_columnNo, "<>"));
			readChar();
		}
		else {
			m_tokenVec.push_back(token(token::LT, m_lineNo, m_columnNo, "<"));
		}
		return true;
	}
	else if (m_currentChar == '\'') {
		m_tokenVec.push_back(token(token::SINQS, m_lineNo, m_columnNo, "'"));
		stringstream stream;
		while (true) {
			readChar();
			if (m_currentChar == '\'') {
				m_tokenVec.push_back(token(token::CHAR_VALUE, m_lineNo, m_columnNo, stream.str()));
				m_tokenVec.push_back(token(token::SINQS, m_lineNo, m_columnNo, "'"));
				readChar();
				break;
			}
			else if (m_currentChar == '\n' || m_isEOF) {
				m_tokenVec.push_back(token(token::CHAR_VALUE, m_lineNo, m_columnNo, stream.str()));
				break;
			}
			else {
				stream << m_currentChar;
			}
		}
		return true;
	}
	else if (m_currentChar == '"') {
		m_tokenVec.push_back(token(token::DOUQS, m_lineNo, m_columnNo, "\""));
		stringstream stream;
		while (true) {
			readChar();
			if (m_currentChar == '"') {
				m_tokenVec.push_back(token(token::STR_VALUE, m_lineNo, m_columnNo, stream.str()));
				m_tokenVec.push_back(token(token::DOUQS, m_lineNo, m_columnNo, "\""));
				readChar();
				break;
			}
			else if (m_currentChar == '\n' || m_isEOF) {
				m_tokenVec.push_back(token(token::STR_VALUE, m_lineNo, m_columnNo, stream.str()));
				break;
			}
			else if (m_currentChar == '\\') {
				readChar();
				if (m_currentChar == 't')
					stream << '\t';
				else if (m_currentChar == 'n')
					stream << '\n';
				else stream << m_currentChar;
			}
			else {
				stream << m_currentChar;
			}
		}
		return true;
	}
	else {
		return false;
	}
		
}

//是否为数字
//return: 是返回true，否返回false
bool lexer::isDigit() {
	if (isdigit(m_currentChar)) {
		bool isReal = false;
		stringstream stream;
		stream << m_currentChar;
		readChar();
		if (isalpha(m_currentChar)) {
			errorHandle(stream);
			errorVec.push_back(2);
			stream << "(行号: " << m_lineNo << " 列号: " << m_columnNo << ")";
			m_tokenVec.push_back(token(token::ERR, m_lineNo, m_columnNo, errorStr(2)+stream.str()));
			return false;
		}
		while (isdigit(m_currentChar) || m_currentChar == '.') {
			if (m_currentChar == '.') {
				if (isReal)
					break;
				else
					isReal = true;
			}
			stream << m_currentChar;
			readChar();
			if (isalpha(m_currentChar)) {
				errorHandle(stream);
				errorVec.push_back(2);
				stream << "(行号: " << m_lineNo << " 列号: " << m_columnNo << ")";
				m_tokenVec.push_back(token(token::ERR, m_lineNo, m_columnNo, errorStr(2) + stream.str()));
				return false;
			}
		}
		if (isReal)
			m_tokenVec.push_back(token(token::REAL_VALUE, m_lineNo, m_columnNo, stream.str()));
		else
			m_tokenVec.push_back(token(token::INT_VALUE, m_lineNo, m_columnNo, stream.str()));
		return true;
	}
	else {
		return false;
	}
}

//是否为保留字或标识符
//return: 是返回true，否返回false
bool lexer::isReserveWordOrID() {
	if (isalpha(m_currentChar) || m_currentChar == '_') {
		stringstream stream;
		stream << m_currentChar;
		readChar();
		while (isalnum(m_currentChar) || m_currentChar == '_') {
			stream << m_currentChar;
			readChar();
		}
		string word = stream.str();
		if (word.at(word.size() - 1) == '_') {
			errorVec.push_back(1);
			stream << "(行号: " << m_lineNo << " 列号: " << m_columnNo << ")";
			m_tokenVec.push_back(token(token::ERR, m_lineNo, m_columnNo, errorStr(1)));

		}
		else if (word == "if") {
			m_tokenVec.push_back(token(token::IF, m_lineNo, m_columnNo, word));
		}
		else if (word == "else") {
			m_tokenVec.push_back(token(token::ELSE, m_lineNo, m_columnNo, word));
		}
		else if (word == "while") {
			m_tokenVec.push_back(token(token::WHILE, m_lineNo, m_columnNo, word));
		}
		else if (word == "for") {
			m_tokenVec.push_back(token(token::FOR, m_lineNo, m_columnNo, word));
		}
		else if (word == "read") {
			m_tokenVec.push_back(token(token::READ, m_lineNo, m_columnNo, word));
		}
		else if (word == "write") {
			m_tokenVec.push_back(token(token::WRITE, m_lineNo, m_columnNo, word));
		}
		else if (word == "int") {
			m_tokenVec.push_back(token(token::INT, m_lineNo, m_columnNo, word));
		}
		else if (word == "real") {
			m_tokenVec.push_back(token(token::REAL, m_lineNo, m_columnNo, word));
		}
		else if (word == "char") {
			m_tokenVec.push_back(token(token::CHAR, m_lineNo, m_columnNo, word));
		}
		else if (word == "void") {
			m_tokenVec.push_back(token(token::VOI, m_lineNo, m_columnNo, word));
		}
		else if (word == "return") {
			m_tokenVec.push_back(token(token::RETURN, m_lineNo, m_columnNo, word));
		}
		else {
			m_tokenVec.push_back(token(token::ID, m_lineNo, m_columnNo, word));
		}
		return true;
	}
	else {
		return false;
	}
}
