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
	vector<quarterExp> generateCode(vector<treeNode *>& treeNodeVec, string & errStr, bool & isError);
	static int programEntry;
	int getEntry();

private:

	int m_level;					//层数
	int m_curFunLevel;				//函数层
	int m_curStmtLineNo;			//当前语句行数
	vector<quarterExp> m_qeVec;		//四元式
	symbolTable m_table;			//符号表


	stack<int> m_typeStack;			//类型栈

	vector<int> m_returnJmpIndexVec;//返回集

	void interpret(treeNode * node);				//总解释函数

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

	//将function类型转为Value类型
	int transformFunToValue(int funType);

};

