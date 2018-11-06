#include "parser.h"
#include <queue>

//�﷨�������
//parameter: ���Ķ����еĴʷ��������
//return: �﷨������
vector<treeNode*> parser::SyntacticAnalyse(vector<token> tokenVec)
{
	index = 0;
	m_tokenVec = tokenVec;
	m_treeNodeVec.clear();
	m_errorNode = NULL;
	try {
		while (index < m_tokenVec.size()) {
			treeNode* node = parseStmt();
			if (node == NULL)
				continue;
			m_treeNodeVec.push_back(node);
		}
	}
	catch (parseexception & pe) {
		errorStr = pe.message();
		//pe.addToErrorMap();
		cout << errorStr << endl;
		if (m_errorNode != NULL)
			delete m_errorNode;
	}
	return m_treeNodeVec;
}

parser::parser()
{
}


parser::~parser()
{
}


//��������
treeNode * parser::parseStmt()
{
	switch(GetNextTokenType()) {
		case token::LCOM:
			return parseStmt();
		case token::SCOM:
			return parseStmt();
		case token::IF:
			return parseIfStmt();
		case token::WHILE:
			return parseWhileStmt();
		case token::FOR:
			return parseForStmt();
		case token::READ:
			return parseReadStmt();
		case token::WRITE:
			return parseWriteStmt();
		case token::INT:
		case token::REAL:
		case token::CHAR:
		case token::VOI:
			return parseDeclareStmt(false);
		case token::LBRACE:
			return parseStmtBlock();
		case token::ID:
			if (GetNextNextTokenType() == token::LPS)
				return pareseFunctionCall();
			else
				return parseAssignStmt(false);
		case token::RETURN:
			return parseReturn();
		default:
			throw parseexception("statement", token(GetNextTokenType(), GetNextTokenColumnNo(), GetNextTokenLineNo()));
	}
}

/**
* if���
* left���exp���ʽ
* middle���if������ȷʱ��TreeNode
* right���else��TreeNode
*/
treeNode * parser::parseIfStmt()
{
	treeNode * node = new treeNode(treeNode::IF_STMT);
	ConsumeNextToken(token::IF);
	ConsumeNextToken(token::LPS);
	node->setLeft(parseExp());
	ConsumeNextToken(token::RPS);
	node->setMiddle(parseStmt());
	if (GetNextTokenType() == token::ELSE) {
		ConsumeNextToken(token::ELSE);
		node->setRight(parseStmt());
	}
	return node;
}

/**
* left�洢EXP
* middle�洢ѭ����
*/
treeNode * parser::parseWhileStmt()
{
	treeNode* node = new treeNode(treeNode::WHILE_STMT);
	ConsumeNextToken(token::WHILE);
	ConsumeNextToken(token::LPS);
	node->setLeft(parseExp());
	ConsumeNextToken(token::RPS);
	node->setMiddle(parseStmt());
	return node;
}

/**
* for���
* left��ű�����ֵ���
* middle����ж����
* right��ű�����ֵ���
* middle����next�����for���ĺ�����
*/
treeNode * parser::parseForStmt()
{
	treeNode * node = new treeNode(treeNode::FOR_STMT);
	ConsumeNextToken(token::FOR);
	node->setLineNo(m_currentToken.lineNoValue());
	ConsumeNextToken(token::LPS);
	node->setLeft(parseAssignStmt(true));
	ConsumeNextToken(token::SEMI);
	node->setMiddle(parseExp());
	ConsumeNextToken(token::SEMI);
	node->setRight(parseAssignStmt(true));
	ConsumeNextToken(token::RPS);
	node->getMiddle()->setNext(parseStmtBlock());
	return node;
}

/**
* read���
* left�洢һ��VAR
*/
treeNode * parser::parseReadStmt()
{
	treeNode* node = new treeNode(treeNode::READ_STMT);
	ConsumeNextToken(token::READ);
	ConsumeNextToken(token::LPS);
	node->setLeft(parseVariableName());
	ConsumeNextToken(token::RPS);
	ConsumeNextToken(token::SEMI);
	return node;
}

/**
* write���
* left�洢һ��EXP
*/
treeNode * parser::parseWriteStmt()
{
	treeNode* node = new treeNode(treeNode::WRITE_STMT);
	ConsumeNextToken(token::WRITE);
	ConsumeNextToken(token::LPS);
	node->setLeft(parseExp());
	ConsumeNextToken(token::RPS);
	ConsumeNextToken(token::SEMI);
	return node;
}

/**
* �������
* left�д��VAR�ڵ�
* ����и�ֵEXP,������middle��
*/
treeNode * parser::parseDeclareStmt(bool IsInsideFunction)
{
	treeNode * node = new treeNode(treeNode::DECLARE_STMT);
	int type = GetNextTokenType();
	int types[4] = { token::INT,token::REAL,token::CHAR ,token::VOI};
	for (int i = 0; i < 4; i++) {
		if (type == types[i]) {
			ConsumeNextToken(type);
			break;
		}
	}
	if (GetNextNextTokenType() == token::LPS) //��������
	{
		node->setLeft(parseFunctionDeclare());
		node->setMiddle(parseStmtBlock());
	}
	else //��������
	{
		node->setLeft(parseVariableName());
		if (GetNextTokenType() == token::ASSIGN)//
		{
			ConsumeNextToken(token::ASSIGN);
			node->setMiddle(parseExp());
		}
		if(!IsInsideFunction)
			ConsumeNextToken(token::SEMI);
	}
	return node;
}

/**
* ��ֵ���
* left���var�ڵ�
* middle���exp�ڵ�
*/
treeNode * parser::parseAssignStmt(bool IsInsideFunction)
{
	treeNode * node = new treeNode(treeNode::ASSIGN_STMT);
	node->setLeft(parseVariableName());
	ConsumeNextToken(token::ASSIGN);
	node->setMiddle(parseExp());
	if (!IsInsideFunction)
		ConsumeNextToken(token::SEMI);
	return node;
}

/**
* ������
* next��ź����е���һ��
*/
treeNode * parser::parseStmtBlock()
{
	treeNode * node = new treeNode(treeNode::STMT_BLOCK);
	ConsumeNextToken(token::LBRACE);
	treeNode * header = node;
	treeNode * temp;
	while (GetNextTokenType() != token::RBRACE) {
		temp = parseStmt();
		header->setNext(temp);
		header = temp;
	}
	ConsumeNextToken(token::RBRACE);
	return node;
}

/**
* ���ʽ
* left��Ŷ���ʽ
* middle����߼������
* right��Ŷ���ʽ
*/
treeNode * parser::parseExp()
{
	treeNode * node = new treeNode(treeNode::EXP);
	node->setDataType(token::LOGIC_EXP);
	treeNode * leftNode = parsePolynomial();
	int types[6] = { token::EQ, token::NEQ, token::GT, token::GET, token::LT, token::LET };
	for (int i = 0; i < 6; i++) {
		if (GetNextTokenType() == types[i]) {
			node->setLeft(leftNode);
			node->setMiddle(parseLogicOp());
			node->setRight(parsePolynomial());
			return node;
		}
	}
	return leftNode;
}

/**
* ����ʽ
* left�����
* middle��żӼ������
* right��Ŷ���ʽ
*/
treeNode * parser::parsePolynomial()
{
	treeNode * node = new treeNode(treeNode::ADDTIVE_EXP);
	node->setDataType(token::ADDTIVE_EXP);
	treeNode * leftNode = parseTerm();
	if (GetNextTokenType() == token::PLUS || GetNextTokenType() == token::MINUS) {
		node->setLeft(leftNode);
		node->setMiddle(parseAddMinOp());
		node->setRight(parsePolynomial());
		return node;
	}
	return leftNode;
}

/**
* ��
* left�������
* middle��ų˳������
* right�����
*/
treeNode * parser::parseTerm()
{
	treeNode * node = new treeNode(treeNode::TERM);
	node->setDataType(token::TERM_EXP);
	treeNode * leftNode = parseFactor();
	if (GetNextTokenType() == token::DIV || GetNextTokenType() == token::MUL) {
		node->setLeft(leftNode);
		node->setMiddle(parseMutDivOp());
		node->setRight(parseTerm());
		return node;
	}
	return leftNode;
}

/**
* ����
* left���һ��TreeNode
* ����Ǹ�TreeNode��var,����һ������/����Ԫ��
* ������TreeNode��exp,����һ�����ʽ����
* �����LITREAL,��LITREAL��value�д������ֵ���ַ���ʽ
*/
treeNode * parser::parseFactor()
{
	treeNode * node = new treeNode(treeNode::FACTOR);
	switch (GetNextTokenType()) {
	case token::INT_VALUE:
	case token::REAL_VALUE:
	case token::SINQS:
	case token::DOUQS:
		node->setLeft(pareseLiteral());
		break;
	case token::LPS:
		ConsumeNextToken(token::LPS);
		node = parseExp();
		ConsumeNextToken(token::RPS);
		break;
	case token::MINUS:
		node->setDataType(token::MINUS);
		ConsumeNextToken(token::MINUS);
		node->setLeft(parseTerm());
		break;
	case token::PLUS:
		ConsumeNextToken(token::PLUS);
		node->setLeft(parseTerm());
		break;
	default:
		if (GetNextNextTokenType() == token::LPS)
			node->setLeft(parseFunctionDeclare());
		else
			node->setLeft(parseVariableName());
		break;
	}
	return node;
}

/**
* ����ֵ
* value�д������ֵ,�޷���
* datatype�������,��TOKEN��
*/
treeNode * parser::pareseLiteral()
{
	treeNode * node = new treeNode(treeNode::LITERAL);
	switch (GetNextTokenType()) {
	case token::INT_VALUE:
		ConsumeNextToken(token::INT_VALUE);
		node->setDataType(m_currentToken.typeValue());
		node->setValue(m_currentToken.value());
		break;
	case token::REAL_VALUE:
		ConsumeNextToken(token::REAL_VALUE);
		node->setDataType(m_currentToken.typeValue());
		node->setValue(m_currentToken.value());
		break;
	case token::SINQS:
		ConsumeNextToken(token::SINQS);
		ConsumeNextToken(token::CHAR_VALUE);
		node->setDataType(token::CHAR_VALUE);
		node->setValue("'" + m_currentToken.value() + "'");
		ConsumeNextToken(token::SINQS);
		break;
	case token::DOUQS:
		ConsumeNextToken(token::DOUQS);
		ConsumeNextToken(token::STR_VALUE);
		node->setDataType(token::STR_VALUE);
		node->setValue("\"" + m_currentToken.value() + "\"");
		ConsumeNextToken(token::DOUQS);
		break;
	default:
		throw("literals", token(GetNextTokenType(), GetNextTokenColumnNo(), GetNextTokenLineNo()));
	}
	return node;
}

/**
* �߼������
*/
treeNode * parser::parseLogicOp()
{
	treeNode * node = new treeNode(treeNode::OP);
	int types[6] = { token::EQ, token::NEQ, token::GT, token::GET, token::LT, token::LET };
	int type;
	for (int i = 0; i < 6; i++) {
		if (types[i] == GetNextTokenType()) {
			type = types[i];
			break;
		}
	}
	ConsumeNextToken(type);
	node->setDataType(m_currentToken.typeValue());
	return node;
}

/**
* �Ӽ������
*/
treeNode * parser::parseAddMinOp()
{
	treeNode * node = new treeNode(treeNode::OP);
	int types[2] = { token::PLUS, token::MINUS };
	int type = GetNextTokenType();
	if (type == types[0] || type == types[1]) {
		ConsumeNextToken(type);
	}
	node->setDataType(m_currentToken.typeValue());
	return node;
}

/**
* �˳������
*/
treeNode * parser::parseMutDivOp()
{
	treeNode * node = new treeNode(treeNode::OP);
	int types[2] = { token::MUL, token::DIV };
	int type = GetNextTokenType();
	if (type == types[0] || type == types[1]) {
		ConsumeNextToken(type);
	}
	node->setDataType(m_currentToken.typeValue());
	return node;
}

/**
* ����
* datatype��ű�������Token.INT �� REAL
* value��ű�����
* left:
* ����������б�����leftֵ�����������exp,�������ĵ����б�����left�����������ֵexp,��Ϊnull,��˵���ǵ����ı���,��������
* ���洢ֵ
*/
treeNode * parser::parseVariableName()
{
	treeNode * node = new treeNode(treeNode::VAR);
	ConsumeNextToken(token::ID);
	node->setValue(m_currentToken.value());
	node->setLineNo(m_currentToken.lineNoValue());
	if (GetNextTokenType() == token::LBRACKET) {
		ConsumeNextToken(token::LBRACKET);
		node->setLeft(parseExp());
		ConsumeNextToken(token::RBRACKET);
	}
	return node;
}

/**
* ��������
* left��ŵ�һ������
* left���ӽڵ��д���Ž������Ĳ���
*/
treeNode * parser::parseFunctionDeclare()
{
	treeNode * node = new treeNode(treeNode::FUNDECLARE);
	ConsumeNextToken(token::ID);
	node->setValue(m_currentToken.value());
	node->setLineNo(m_currentToken.lineNoValue());
	ConsumeNextToken(token::LPS);
	treeNode * header, *temp;
	if (GetNextTokenType() == token::INT || GetNextTokenType() == token::REAL || GetNextTokenType() == token::CHAR) {
		header = parseDeclareStmt(true);
		node->setLeft(header);
		while (GetNextTokenType() != token::RPS) {
			ConsumeNextToken(token::COMMA);
			temp = parseDeclareStmt(true);
			header->setNext(temp);
			header = temp;
		}
	}
	ConsumeNextToken(token::RPS);
	return node;
}

/**
* ��������
* left��ŵ�һ���������ʽ
* left���ӽڵ��д�Ž������Ĳ������ʽ
*/
treeNode * parser::pareseFunctionCall()
{
	treeNode * node = new treeNode(treeNode::FUNCALL);
	ConsumeNextToken(token::ID);
	node->setLineNo(m_currentToken.lineNoValue());
	node->setValue(m_currentToken.value());
	ConsumeNextToken(token::LPS);
	treeNode * header, *temp;
	if (GetNextTokenType() != token::RPS) {
		header = parseExp();
		node->setLeft(header);
		while (GetNextTokenType() != token::RPS) {
			ConsumeNextToken(token::COMMA);
			temp = parseExp();
			header->setNext(temp);
			header = temp;
		}
	}
	ConsumeNextToken(token::RPS);
	ConsumeNextToken(token::SEMI);
	return node;
}

treeNode * parser::parseVariableType()
{
	return nullptr;
}

/**
* return���
* left��ű��ʽ
*/
treeNode * parser::parseReturn()
{
	treeNode * node = new treeNode(treeNode::RETURN_STMT);
	ConsumeNextToken(token::RETURN);
	node->setLeft(parseExp());
	node->setLineNo(m_currentToken.lineNoValue());
	ConsumeNextToken(token::SEMI);
	return node;
}

//������һ��token
void parser::ConsumeNextToken(int type)
{
	if (index >= m_tokenVec.size())
		throw parseexception(type, token(token::NULLL, m_currentToken.lineNoValue(), m_currentToken.columnNoValue()));

	if (GetNextTokenType() == type) {
		m_currentToken = m_tokenVec[index];
		index++;
		return;
	}
	throw parseexception(type, m_currentToken);
}


//���token����
int parser::GetNextTokenType()
{
	if (index < m_tokenVec.size()) {
		int type = m_tokenVec[index].typeValue();
		if (type == token::LCOM) {
			index += 2;
			if (index< m_tokenVec.size()) {
				type = m_tokenVec[index].typeValue();
			}
			else {
				return token::NULLL;
			}
		}
		else if (type == token::SCOM) {
			index++;
			if (index< m_tokenVec.size()) {
				type = m_tokenVec[index].typeValue();
			}
			else {
				return token::NULLL;
			}
		}
		return type;
	}
	return token::NULLL;
}

//�������һ��token����
int parser::GetNextNextTokenType()
{
	if (index + 1 < m_tokenVec.size()) {
		int type = m_tokenVec[index + 1].typeValue();
		if (type == token::LCOM) {
			index += 2;
			if (index + 1 < m_tokenVec.size()) {
				type = m_tokenVec[index + 1].typeValue();
			}
			else {
				return token::NULLL;
			}
		}
		else if (type == token::SCOM) {
			index++;
			if (index + 1 < m_tokenVec.size()) {
				type = m_tokenVec[index + 1].typeValue();
			}
			else {
				return token::NULLL;
			}
		}
		return type;
	}
	return token::NULLL;
}

//���token�к�
int parser::GetNextTokenLineNo()
{
	if (index < m_tokenVec.size())
		return m_tokenVec[index].lineNoValue();
	else
		return m_currentToken.lineNoValue();
}

//���token�к�
int parser::GetNextTokenColumnNo() {
	if (index < m_tokenVec.size())
		return m_tokenVec[index].columnNoValue();
	else
		return m_currentToken.columnNoValue();
}
