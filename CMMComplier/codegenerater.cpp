#include "codegenerater.h"
#include <sstream>
#include "common.h"
#include "codegeneraterException.h"
#include <string>


codegenerater::codegenerater()
{
}


codegenerater::~codegenerater()
{
}

vector<quarterExp> codegenerater::generateCode(vector<treeNode*>& treeNodeVec, string & errStr)
{
	globalVarVec.clear();
	quarterExp::setIndex(-1);
	m_level = 0;
	m_curStmtLineNo = 0;
	m_curFunLevel = 0;
	m_table.clearVec();
	m_qeVec.clear();
	m_returnJmpIndexVec.clear();
	while (!m_typeStack.empty())
		m_typeStack.pop();
	for (int i = 0; i < treeNodeVec.size(); i++) {
		try {
			interpret(treeNodeVec[i]);
		}
		catch (codegeneraterException & cge) {
			errStr += ("\n   " + cge.message());
		}
		
	}
	return m_qeVec;
}

void codegenerater::interpret(treeNode * node)
{
	while (true) {
		m_curStmtLineNo = node->getLineNo();
		if (node->getType() != treeNode::DECLARE_STMT && m_level == 0)
			throw codegeneraterException(m_curStmtLineNo, "非声明语句");
		switch (node->getType()) {
		case treeNode::IF_STMT:
			interpretIfStmt(node);
			break;
		case treeNode::WHILE_STMT:
			interpretWhileStmt(node);
			break;
		case treeNode::FOR_STMT:
			interpretForStmt(node);
			break;
		case treeNode::READ_STMT:
			interpretReadStmt(node);
			break;
		case treeNode::WRITE_STMT:
			interpretWriteStmt(node);
			break;
		case treeNode::DECLARE_STMT:
			interpretDecalreStmt(node);
			break;
		case treeNode::ASSIGN_STMT:
			interpretAssignStmt(node);
			break;
		case treeNode::FUNCALL:
			interpretFunctionCall(node);
			break;
		case treeNode::RETURN_STMT:
			interpretReturnStmt(node);
			break;
		default:
			break;
		}
		m_table.clearTempVec();
		if (node->getNext() != NULL)
			node = node->getNext();
		else break;
	}
}

/*
if语句四元式
(JMP, 判断表达式, , )
(IN, , , )
(函数主体操作)
(OUT, , ,)
如果有else则进行下面操作
(JMP, , ,) 
(IN, , ,)
(OUT, , ,)
*/

void codegenerater::interpretIfStmt(treeNode * node)
{
	m_qeVec.push_back(quarterExp(quarterExp::JMP, interpretExp(node->getLeft()), "", "", m_curStmtLineNo));
	int ifFalseJmpIndex = quarterExp::index();
	m_qeVec.push_back(quarterExp(quarterExp::IN, "", "", "", m_curStmtLineNo));
	m_level++;
	interpret(node->getMiddle());
	m_table.deregisterSymbol(m_level);
	m_level--;
	m_qeVec.push_back(quarterExp(quarterExp::OUT, "", "", "", m_curStmtLineNo));
	if (node->getRight() != NULL) {
		m_qeVec.push_back(quarterExp(quarterExp::JMP, "", "", "", m_curStmtLineNo));
		int outJmpIndex = quarterExp::index();
		m_qeVec[ifFalseJmpIndex].setFourth(itos(quarterExp::index() + 1));
		m_qeVec.push_back(quarterExp(quarterExp::IN, "", "", "", m_curStmtLineNo));
		m_level++;
		interpret(node->getRight());
		m_qeVec.push_back(quarterExp(quarterExp::OUT, "", "", "", m_curStmtLineNo));
		m_table.deregisterSymbol(m_level);
		m_level--;
		m_qeVec[outJmpIndex].setFourth(itos(quarterExp::index() + 1));
	}
	else {
		m_qeVec[ifFalseJmpIndex].setFourth(itos(quarterExp::index() + 1));
	}
}

/*
while语句四元式
(JMP,判断表达式, ,)
(IN, , ,)
(函数主体操作)
(OUT, , ,)
(JMP, , ,返回函数主体)
*/
void codegenerater::interpretWhileStmt(treeNode * node)
{
	int jumpLine = quarterExp::index() + 1;
	m_qeVec.push_back(quarterExp(quarterExp::JMP, interpretExp(node->getLeft()),"","",m_curStmtLineNo));
	int falseJmpIndex = quarterExp::index();
	m_qeVec.push_back(quarterExp(quarterExp::IN, "", "", "", m_curStmtLineNo));
	m_level++;
	interpret(node->getMiddle());
	m_table.deregisterSymbol(m_level);
	m_level--;
	m_qeVec.push_back(quarterExp(quarterExp::OUT, "", "", "", m_curStmtLineNo));
	m_qeVec.push_back(quarterExp(quarterExp::JMP, "", "", itos(jumpLine), m_curStmtLineNo));
	m_qeVec[falseJmpIndex].setFourth(itos(quarterExp::index() + 1));
}

/*
for语句四元式
(JMP,判断表达式, , )
(IN, , ,)
(函数主体操作)
(OUT, , ,)
(JMP,函数主体行数, , )
*/
void codegenerater::interpretForStmt(treeNode * node)
{
	interpret(node->getLeft());
	int jumpLine = quarterExp::index() + 1;
	m_qeVec.push_back(quarterExp(quarterExp::JMP, interpretExp(node->getMiddle()), "", "", m_curStmtLineNo));
	int falseJmpIndex = quarterExp::index();
	m_qeVec.push_back(quarterExp(quarterExp::IN, "", "", "", m_curStmtLineNo));
	m_level++;
	interpret(node->getMiddle()->getNext());
	interpret(node->getRight());
	m_table.deregisterSymbol(m_level);
	m_level--;
	m_qeVec.push_back(quarterExp(quarterExp::OUT, "", "", "", m_curStmtLineNo));
	m_qeVec.push_back(quarterExp(quarterExp::JMP, "", "", itos(jumpLine), m_curStmtLineNo));
	m_qeVec[falseJmpIndex].setFourth(itos(quarterExp::index() + 1));
}

/*
read语句四元式
(READ, , ,存储变量)
*/
void codegenerater::interpretReadStmt(treeNode * node)
{
	int varType = m_table.checkSymbolIsDeclared(node->getLeft(), node->getLeft()->getLineNo());
	string varStr = node->getLeft()->getValue();
	switch (varType) {
	case 1:
	case -1:
	case 2:
		throw codegeneraterException(m_curStmtLineNo, "read函数的参数类型不匹配");
		break;
	}
	m_qeVec.push_back(quarterExp(quarterExp::READ, "", "", varStr, m_curStmtLineNo));
}

/*
write语句四元式
(WRITE, , ,输出变量)
*/
void codegenerater::interpretWriteStmt(treeNode * node)
{
	string varStr = interpretExp(node->getLeft());
	m_typeStack.pop();
	m_qeVec.push_back(quarterExp(quarterExp::WRITE, "", "", varStr, m_curStmtLineNo));
}

/*
声明变量语句四元式
(INT/REAL/CHAR, 值, , 变量)
*/
void codegenerater::interpretDecalreStmt(treeNode * node)
{
	if (node->getLeft()->getType() == treeNode::FUNDECLARE) {
		interpretFunction(node);
		return;
	}
	int startStmtIndex = quarterExp::index() + 1;
	treeNode * var = node->getLeft();
	if (var->getLeft() == NULL) {
		string value = "";
		if (node->getMiddle() != NULL) {
			if (node->getMiddle()->getType()==treeNode::REG) {
				value = node->getMiddle()->getValue();
			}
			else {
				value = interpretExp(node->getMiddle());
				checkDeclareIsRight(var->getDataType());
			}
		}
		if (var->getDataType() == token::INT) {
			m_qeVec.push_back(quarterExp(quarterExp::INT, value, "", var->getValue(), m_curStmtLineNo));
			symbol symbol(var->getValue(), symbol::SINGLE_INT, var->getLineNo(), m_level);
			m_table.registerSymbol(symbol);
		}
		else if (var->getDataType() == token::REAL) {
			m_qeVec.push_back(quarterExp(quarterExp::REAL, value, "", var->getValue(), m_curStmtLineNo));
			symbol symbol(var->getValue(), symbol::SINGLE_REAL, var->getLineNo(), m_level);
			m_table.registerSymbol(symbol);
		}
		else if (var->getDataType() == token::CHAR) {
			m_qeVec.push_back(quarterExp(quarterExp::CHAR, value, "", var->getValue(), m_curStmtLineNo));
			symbol symbol(var->getValue(), symbol::SINGLE_CHAR, var->getLineNo(), m_level);
			m_table.registerSymbol(symbol);
		}
	}
	if (m_level == 0) {
		int endStmtIndex = quarterExp::index();
		for (int i = startStmtIndex; i <= endStmtIndex; i++)
			globalVarVec.push_back(i);
	}

}

/*
赋值语句四元式
(ASSIGN,值, ,变量)
*/
void codegenerater::interpretAssignStmt(treeNode * node)
{
	int varType = m_table.checkSymbolIsDeclared(node->getLeft(), node->getLeft()->getLineNo());
	int targetType = -1;
	string varStr = node->getLeft()->getValue();
	switch (varType) {
	case 0:
		targetType = value::SINGLE_VALUE;
		break;
	default:
		break;
	}
	string value = interpretExp(node->getMiddle());
	checkAssignIsRight(targetType);
	m_qeVec.push_back(quarterExp(quarterExp::ASSIGN, value, "", varStr, m_curStmtLineNo));
}

/*
函数声明四元式
(INT/REAL/VOID/CHAR, , ,函数名())
(IN, , ,)
(函数主体操作)
(OUT, , ,)
(JMP, , ,返回调用函数前的语句序号)
*/
void codegenerater::interpretFunction(treeNode * node)
{
	if (m_level > 0)
		throw codegeneraterException(m_curStmtLineNo, "无法在函数内部定义函数");
	treeNode * var = node->getLeft();
	int symbolType;
	string quarterExpType;
	switch (var->getDataType()) {
	case token::INT:
		symbolType = funsymbol::INT;
		quarterExpType = quarterExp::INT;
		break;
	case token::REAL:
		symbolType = funsymbol::REAL;
		quarterExpType = quarterExp::REAL;
		break;
	case token::CHAR:
		symbolType = funsymbol::CHAR;
		quarterExpType = quarterExp::CHAR;
		break;
	case token::VOI:
		symbolType = funsymbol::VOID;
		quarterExpType = quarterExp::VOID;
		break;
	default:
		break;
	}
	if (var->getValue() == "main") {
		codegenerater::programEntry = quarterExp::index() + 1;
		//codegenerater::programEntry = m_qeVec.size();
	}
	m_qeVec.push_back(quarterExp(quarterExpType, "", "", var->getValue() + "()", m_curStmtLineNo));
	funsymbol funsymol(var->getValue(), var->getLineNo(), symbolType, quarterExp::index() + 1);
	m_qeVec.push_back(quarterExp(quarterExp::IN, "", "", "", m_curStmtLineNo));
	m_level++;
	m_curFunLevel = m_level;
	treeNode * argNode = var->getLeft();
	while (argNode != NULL) {
		if (var->getValue() == "main")
			throw codegeneraterException(m_curStmtLineNo, "无法对main函数赋参数");
		interpretDecalreStmt(argNode);
		funsymol.m_argTypeVec.push_back(m_table.getLastSymbolType());
		argNode = argNode->getNext();
	}
	m_table.registerFunSymbol(funsymol);
	interpret(node->getMiddle());
	m_table.deregisterSymbol(m_level);
	m_level--;
	m_qeVec.push_back(quarterExp(quarterExp::OUT, "", "", "", m_curStmtLineNo));
	for (int i = 0; i < m_returnJmpIndexVec.size(); i++)
		m_qeVec[m_returnJmpIndexVec[i]].setFourth(itos(quarterExp::index()));
	m_returnJmpIndexVec.clear();
	m_qeVec.push_back(quarterExp(quarterExp::JMP, "", "", "@ebp", m_curStmtLineNo));
}

/*
return语句四元式
(ASSIGN,返回值, ,函数返回所赋的变量)
(JMP, ,返回调用函数时的四元式序号)
*/
void codegenerater::interpretReturnStmt(treeNode * node)
{
	if (m_level == 0)
		throw codegeneraterException(m_curStmtLineNo, "应输入声明语句");
	if (m_table.getCurFunSymbolType() == funsymbol::VOID)
		throw codegeneraterException(m_curStmtLineNo, "函数不需要返回值");
	string result = interpretExp(node->getLeft());
	int valueType = transformFunToValue(m_table.getCurFunSymbolType());
	checkAssignIsRight(valueType);
	m_qeVec.push_back(quarterExp(quarterExp::ASSIGN, result, "", "@ebp-1", m_curStmtLineNo));
	for (int i = m_level; i > m_curFunLevel; i--)
		m_qeVec.push_back(quarterExp(quarterExp::OUT, "", "", "", m_curStmtLineNo));
	m_qeVec.push_back(quarterExp(quarterExp::JMP, "", "", "", m_curStmtLineNo));
	m_returnJmpIndexVec.push_back(quarterExp::index());
}

/*
函数调用四元式
(ASSIGN,0, ,栈顶)
(ASSIGN,依次将参数赋值到栈顶)
(ASSIGN,函数起始地址 , ,栈顶)
(CALL, , ,函数名)
(JMP, , ,函数所在四元式序号)
(CALLFH, , ,)
*/
string codegenerater::interpretFunctionCall(treeNode * node)
{
	if (m_level == 0)
		throw codegeneraterException(m_curStmtLineNo, "应输入声明语句");
	funsymbol symbol = m_table.getFunSymbol(node->getValue(), m_curStmtLineNo);
	if (symbol.name() == "main")
		throw codegeneraterException(m_curStmtLineNo, "禁止调用main函数");
	stack<string> argStack;
	treeNode * argNode = node->getLeft();
	int index = 0;
	while (argNode != NULL) {
		if (index >= symbol.m_argTypeVec.size())
			throw codegeneraterException(m_curStmtLineNo, "函数参数过多");
		int targetType = -1;
		switch (symbol.m_argTypeVec[index]) {
		case symbol::SINGLE_CHAR:
		case symbol::SINGLE_INT:
		case symbol::SINGLE_REAL:
			targetType = value::SINGLE_VALUE;
			break;
		default:
			break;
		}
		string argValue = interpretExp(argNode);
		checkAssignIsRight(targetType);
		argStack.push(argValue);
		index++;
		argNode = argNode->getNext();
	}
	if (index < symbol.m_argTypeVec.size())
		throw codegeneraterException(m_curStmtLineNo, "函数参数过少");
	m_qeVec.push_back(quarterExp(quarterExp::ASSIGN, "0", "", "@esp", m_curStmtLineNo));
	while (!argStack.empty()) {
		m_qeVec.push_back(quarterExp(quarterExp::ASSIGN, argStack.top(), "", "@esp", m_curStmtLineNo));
		argStack.pop();
	}
	int assignAddrIndex = quarterExp::index() + 1;
	m_qeVec.push_back(quarterExp(quarterExp::ASSIGN, "", "", "@esp", m_curStmtLineNo));
	m_qeVec.push_back(quarterExp(quarterExp::CALL, "", "", symbol.name(), m_curStmtLineNo));
	m_qeVec.push_back(quarterExp(quarterExp::JMP, "", "", itos(symbol.stmtIndex()), m_curStmtLineNo));
	m_qeVec.push_back(quarterExp(quarterExp::CALLFH, "", "", "", m_curStmtLineNo));
	m_qeVec[assignAddrIndex].setSecond("$" + itos(0x10000000 + 4 * quarterExp::index()));

	int valueType = transformFunToValue(symbol.returnType());
	m_typeStack.push(valueType);
	return "@esp-1";
}

/*
表达式语句
*/
string codegenerater::interpretExp(treeNode * node)
{
	if (node->getType() == treeNode::EXP || node->getType() == treeNode::ADDTIVE_EXP) {
		switch (node->getDataType()) {
		case token::LOGIC_EXP:
			return interpretLogicExp(node);
		case token::ADDTIVE_EXP:
			return interpretAddtiveExp(node);
		case token::TERM_EXP:
			return interpretTermExp(node);
		default:
			throw codegeneraterException(m_curStmtLineNo, "表达式不合法");
		}
	}
	else if (node->getType() == treeNode::FACTOR) {
		if (node->getDataType() == token::MINUS) {
			string nodeStr = interpretExp(node->getLeft());
			int type;
			if (isConstant(nodeStr, type))
				return ("-" + nodeStr);
			string temp = m_table.getNewTempSymbolName();
			m_qeVec.push_back(quarterExp(quarterExp::MINUS, "", nodeStr, temp, m_curStmtLineNo));
			return temp;
		}
		else {
			return interpretExp(node->getLeft());
		}
	}
	else if (node->getType() == treeNode::VAR) {
		return interpretVar(node);
	}
	else if (node->getType() == treeNode::LITERAL) {
		if (node->getDataType() == token::STR_VALUE)
			m_typeStack.push(value::STR_VALUE);
		else
			m_typeStack.push(value::SINGLE_VALUE);
		return node->getValue();
	}
	else if (node->getType() == treeNode::FUNCALL) {
		return interpretFunctionCall(node);
	}
	throw codegeneraterException(m_curStmtLineNo, "表达式不合法");
}

/*
逻辑表达式四元式
(符号,左值,右值,临时变量)
*/
string codegenerater::interpretLogicExp(treeNode * node)
{
	int leftType, rightType;
	bool isAllConstant = false;
	string temp = "";
	string leftNodeStr = interpretExp(node->getLeft());
	string rightNodeStr = interpretExp(node->getRight());
	int r_valueType = m_typeStack.top();
	m_typeStack.pop();
	int l_valueType = m_typeStack.top();
	m_typeStack.pop();
	int opType = node->getMiddle()->getDataType();
	if (l_valueType != r_valueType)
		throw codegeneraterException(m_curStmtLineNo, "类型不同，无法进行运算");
	if (l_valueType == value::STR_VALUE && r_valueType == value::STR_VALUE
		&& opType != token::EQ && opType != token::NEQ)
		throw codegeneraterException(m_curStmtLineNo, "无法对字符串进行"+token::typeToString(opType)+"运算");
	m_typeStack.push(value::SINGLE_VALUE);
	if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType))
		isAllConstant = true;
	if (!isAllConstant)
		temp = m_table.getNewTempSymbolName();
	switch (opType) {
	case token::GT:
		if (isAllConstant)
			return value(leftNodeStr, leftType) > value(rightNodeStr, rightType);
		m_qeVec.push_back(quarterExp(quarterExp::GT, leftNodeStr, rightNodeStr, temp, m_curStmtLineNo));
		break;
	case token::GET:
		if (isAllConstant)
			return value(leftNodeStr, leftType) >= value(rightNodeStr, rightType);
		m_qeVec.push_back(quarterExp(quarterExp::GET, leftNodeStr, rightNodeStr, temp, m_curStmtLineNo));
		break;
	case token::LT:
		if (isAllConstant)
			return value(leftNodeStr, leftType) < value(rightNodeStr, rightType);
		m_qeVec.push_back(quarterExp(quarterExp::LT, leftNodeStr, rightNodeStr, temp, m_curStmtLineNo));
		break;
	case token::LET:
		if (isAllConstant)
			return value(leftNodeStr, leftType) <= value(rightNodeStr, rightType);
		m_qeVec.push_back(quarterExp(quarterExp::LET, leftNodeStr, rightNodeStr, temp, m_curStmtLineNo));
		break;
	case token::EQ:
		if (isAllConstant)
			return value(leftNodeStr, leftType) == value(rightNodeStr, rightType);
		m_qeVec.push_back(quarterExp(quarterExp::EQ, leftNodeStr, rightNodeStr, temp, m_curStmtLineNo));
		break;
	case token::NEQ:
		if (isAllConstant)
			return nequal(value(leftNodeStr, leftType),value(rightNodeStr, rightType));
		m_qeVec.push_back(quarterExp(quarterExp::GET, leftNodeStr, rightNodeStr, temp, m_curStmtLineNo));
		break;
	default:
		throw codegeneraterException(m_curStmtLineNo, "表达式非法");
	}
	return temp;
}

/*
多项式四元式
(加减符号,左值,右值,临时变量)
*/
string codegenerater::interpretAddtiveExp(treeNode * node)
{
	int leftType, rightType;
	string leftNodeStr, rightNodeStr, temp_1;
	string addOp = transformAddOp(node->getMiddle()->getDataType());
	if (node->getRight()->getType() == treeNode::FACTOR ||
		node->getRight()->getDataType() == token::TERM_EXP) {
		leftNodeStr = interpretExp(node->getLeft());
		rightNodeStr = interpretExp(node->getRight());
		checkAddtiveExpIsRight(node->getMiddle()->getDataType());
		if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType)) {
			temp_1 = interpretAddtiveExpUtil(addOp, leftNodeStr, rightNodeStr, leftType, rightType);
		}
		else {
			temp_1 = m_table.getNewTempSymbolName();
			m_qeVec.push_back(quarterExp(addOp, leftNodeStr, rightNodeStr, temp_1, m_curStmtLineNo));
		}
	}
	else {
		leftNodeStr = interpretExp(node->getLeft());
		rightNodeStr = interpretExp(node->getRight()->getLeft());
		checkAddtiveExpIsRight(node->getMiddle()->getDataType());
		if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType)) {
			temp_1 = interpretAddtiveExpUtil(addOp, leftNodeStr, rightNodeStr, leftType, rightType);
		}
		else {
			temp_1 = m_table.getNewTempSymbolName();
			m_qeVec.push_back(quarterExp(addOp, leftNodeStr, rightNodeStr, temp_1, m_curStmtLineNo));
		}
		node = node->getRight();
		string temp_2 = "";
		while (node->getRight() != NULL && node->getRight()->getType() != treeNode::FACTOR
			&& node->getRight()->getDataType() != token::TERM_EXP) {
			addOp = transformAddOp(node->getMiddle()->getDataType());
			leftNodeStr = temp_1;
			rightNodeStr = interpretExp(node->getRight()->getLeft());
			checkAddtiveExpIsRight(node->getMiddle()->getDataType());
			if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType)) {
				temp_2 = interpretAddtiveExpUtil(addOp, leftNodeStr, rightNodeStr, leftType, rightType);
			}
			else {
				temp_2 = m_table.getNewTempSymbolName();
				m_qeVec.push_back(quarterExp(addOp, leftNodeStr, rightNodeStr, temp_2, m_curStmtLineNo));
			}
			node = node->getRight();
			temp_1 = temp_2;
		}
		addOp = transformAddOp(node->getMiddle()->getDataType());
		leftNodeStr = temp_1;
		rightNodeStr = interpretExp(node->getRight());
		checkAddtiveExpIsRight(node->getMiddle()->getDataType());
		if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType)) {
			temp_2 = interpretAddtiveExpUtil(addOp, leftNodeStr, rightNodeStr, leftType, rightType);
		}
		else {
			temp_2 = m_table.getNewTempSymbolName();
			m_qeVec.push_back(quarterExp(addOp, leftNodeStr, rightNodeStr, temp_2, m_curStmtLineNo));
		}
		temp_1 = temp_2;
	}
	return temp_1;
}

/*
项四元式
(乘除符号,左值,右值,临时变量)
*/
string codegenerater::interpretTermExp(treeNode * node)
{
	int leftType, rightType;
	string leftNodeStr, rightNodeStr, temp_1;
	string mulOp = transformMulOp(node->getMiddle()->getDataType());
	if (node->getRight()->getType() == treeNode::FACTOR) {
		leftNodeStr = interpretExp(node->getLeft());
		rightNodeStr = interpretExp(node->getRight());
		checkTermIsRight();
		if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType)) {
			temp_1 = interpretTermUtil(mulOp, leftNodeStr, rightNodeStr, leftType, rightType);
		}
		else {
			temp_1 = m_table.getNewTempSymbolName();
			m_qeVec.push_back(quarterExp(mulOp, leftNodeStr, rightNodeStr, temp_1, m_curStmtLineNo));
		}
	}
	else {
		leftNodeStr = interpretExp(node->getLeft());
		rightNodeStr = interpretExp(node->getRight()->getLeft());
		checkTermIsRight();
		if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType)) {
			temp_1 = interpretTermUtil(mulOp, leftNodeStr, rightNodeStr, leftType, rightType);
		}
		else {
			temp_1 = m_table.getNewTempSymbolName();
			m_qeVec.push_back(quarterExp(mulOp, leftNodeStr, rightNodeStr, temp_1, m_curStmtLineNo));
		}
		node = node->getRight();
		string temp_2 = "";
		while (node->getRight() != NULL && node->getRight()->getType() != treeNode::FACTOR) {
			mulOp = transformMulOp(node->getMiddle()->getDataType());
			leftNodeStr = temp_1;
			rightNodeStr = interpretExp(node->getRight()->getLeft());
			checkTermIsRight();
			if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType)) {
				temp_2 = interpretTermUtil(mulOp, leftNodeStr, rightNodeStr, leftType, rightType);
			}
			else {
				temp_2 = m_table.getNewTempSymbolName();
				m_qeVec.push_back(quarterExp(mulOp, leftNodeStr, rightNodeStr, temp_2, m_curStmtLineNo));
			}
			node = node->getRight();
			temp_1 = temp_2;
		}
		mulOp = transformMulOp(node->getMiddle()->getDataType());
		leftNodeStr = temp_1;
		rightNodeStr = interpretExp(node->getRight());
		checkTermIsRight();
		if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType)) {
			temp_2 = interpretTermUtil(mulOp, leftNodeStr, rightNodeStr, leftType, rightType);
		}
		else {
			temp_2 = m_table.getNewTempSymbolName();
			m_qeVec.push_back(quarterExp(mulOp, leftNodeStr, rightNodeStr, temp_2, m_curStmtLineNo));
		}
		temp_1 = temp_2;
	}
	return temp_1;
}

/*
检查变量是否被声明过
*/
string codegenerater::interpretVar(treeNode * node)
{
	int result = m_table.checkSymbolIsDeclared(node, node->getLineNo());
	string varStr = node->getValue();
	if (result == 0)
		m_typeStack.push(value::SINGLE_VALUE);
		
	return varStr;
}


void codegenerater::checkDeclareIsRight(int declarType)
{
	int valueType = m_typeStack.top();
	m_typeStack.pop();
	switch (declarType) {
	case token::INT:
	case token::REAL:
	case token::CHAR:
		if (valueType != value::SINGLE_VALUE)
			throw codegeneraterException(m_curStmtLineNo, "类型不匹配");
		break;
	default:
		break;
	}
}

void codegenerater::checkAssignIsRight(int varType)
{
	int valueType = m_typeStack.top();
	m_typeStack.pop();
	if (varType == value::SINGLE_VALUE) {
		switch (valueType) {
		case value::STR_VALUE:
			throw codegeneraterException(m_curStmtLineNo, "无法将字符串赋值给单值");
		case value::VOID:
			throw codegeneraterException(m_curStmtLineNo, "该函数没有返回值");
		default:
			break;
		}
	}
}

string codegenerater::transformAddOp(int op)
{
	if (op == token::PLUS)
		return quarterExp::PLUS;
	else
		return quarterExp::MINUS;
}

string codegenerater::interpretAddtiveExpUtil(string & addOp, string & leftNodeStr, string & rightNodeStr, int leftType, int rightType)
{
	string result;
	if (addOp == quarterExp::PLUS)
		result = value(leftNodeStr, leftType) + value(rightNodeStr, rightType);
	else
		result = value(leftNodeStr, leftType) - value(rightNodeStr, rightType);
	return result;
}

void codegenerater::checkAddtiveExpIsRight(int opType)
{
	int r_valueType = m_typeStack.top();
	m_typeStack.pop();
	int l_valueType = m_typeStack.top();
	m_typeStack.pop();
	if (l_valueType == value::SINGLE_VALUE && r_valueType == value::SINGLE_VALUE) {
		m_typeStack.push(value::SINGLE_VALUE);
	}
	else {
		throw codegeneraterException(m_curStmtLineNo, "无法对该类型的值进行加减法运算");
	}
}

string codegenerater::transformMulOp(int op)
{
	if (op = token::MUL)
		return quarterExp::MUL;
	else
		return quarterExp::DIV;
}

string codegenerater::interpretTermUtil(string & mulOp, string & leftNodeStr, string & rightNodeStr, int leftType, int rightType)
{
	string result;
	if (mulOp == quarterExp::MUL) {
		result = value(leftNodeStr, leftType)* value(rightNodeStr, rightType);
	}
	else {
		value::callfrom = m_curStmtLineNo;
		result = value(leftNodeStr, leftType) / value(rightNodeStr, rightType);
	}
	return result;
}

void codegenerater::checkTermIsRight()
{
	int r_valueType = m_typeStack.top();
	m_typeStack.pop();
	int l_valueType = m_typeStack.top();
	m_typeStack.pop();
	if (l_valueType == value::SINGLE_VALUE && r_valueType == value::SINGLE_VALUE)
		m_typeStack.push(value::SINGLE_VALUE);
	else
		throw codegeneraterException(m_curStmtLineNo, "无法对该类型的值进行乘除法运算");
}

bool codegenerater::isConstant(string & str, int & type)
{
	type = value::INT_VALUE;
	for (int i = 0; i < str.size(); i++) {
		char ch = str[i];
		if (isalpha(ch) || ch == '_' || ch == '@')//为变量或栈顶底
			return false;
		if (ch == '.')
			type = value::REAL_VALUE;
	}
	return true;
}


int codegenerater::transformFunToValue(int funType)
{
	switch (funType) {
	case funsymbol::VOID:
		return value::VOID;
	case funsymbol::INT:
	case funsymbol::REAL:
	case funsymbol::CHAR:
		return value::SINGLE_VALUE;
	default:
		return value::ADDR;
	}
}

int codegenerater::getEntry() {
	return codegenerater::programEntry;
}

int codegenerater::programEntry = -1;