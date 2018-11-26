#pragma once
#include "quarterExp.h"
#include "treeNode.h"
#include "symbolTable.h"
#include <vector>
#include <string>
#include <stack>
#include <iostream>
#include "value.h"
#include "token.h"
#include <stdlib.h>
using namespace std;
class codegenerater
{
public:
	codegenerater();
	~codegenerater();
	vector<quarterExp> generateCode(vector<treeNode *>& treeNodeVec, string & errStr);
	static int programEntry;
	int getEntry();

private:
	enum {
		ALL,
		VALUE,
		CHAR
	};
	int m_level;
	int m_curFunLevel;
	int m_curStmtLineNo;
	vector<quarterExp> m_qeVec;
	symbolTable m_table;
	


	bool isTypeMatch;
	int m_targetType;

	stack<int> m_typeStack;

	vector<int> m_returnJmpIndexVec;

	void interpret(treeNode * node);

	void interpretIfStmt( treeNode * node);
	void interpretWhileStmt( treeNode * node);
	void interpretForStmt( treeNode * node);
	void interpretReadStmt( treeNode * node);
	void interpretWriteStmt( treeNode * node);
	void interpretDecalreStmt( treeNode * node);
	void interpretAssignStmt( treeNode * node);
	void interpretFunction( treeNode * node);
	void interpretReturnStmt( treeNode * node);

	string interpretFunctionCall( treeNode * node);
	string interpretExp( treeNode * node);
	string interpretLogicExp( treeNode * node);
	string interpretAddtiveExp( treeNode * node);
	string interpretTermExp( treeNode * node);
	string interpretVar( treeNode * node);
	string interpretReference( treeNode * node);
	string interpretScript( treeNode * node);

	//解释declare语句辅助方法
	void checkDeclareIsRight(int declarType);

	//检查赋值类型是否匹配
	void checkAssignIsRight(int varType);

	//解释addtiveExp辅助方法
	string transformAddOp(int op);
	string interpretAddtiveExpUtil( string & addOp,  string & leftNodeStr,  string & rightNodeStr, int leftType, int rightType);
	void checkAddtiveExpIsRight(int opType);

	//解释term辅助方法
	string transformMulOp(int op);
	string interpretTermUtil( string & mulOp,  string & leftNodeStr,  string & rightNodeStr, int leftType, int rightType);
	void checkTermIsRight();

	//判断字符串是否是常量
	bool isConstant(string & str, int & type);

	//判断数组下标是否合法
	bool isSubscriptRight(string & script);

	//判断赋值是否合法
	bool isAssignRight(int varType, string & valueStr);

	//将function类型转为Value类型
	int transformFunToValue(int funType);

};

