#pragma once
#include"treeNode.h"
#include"token.h"
#include<vector>
#include<string>
#include<map>
#include<queue>
#include"parseexception.h"
struct treeNodeString {
	string value;
	int childNum;
};

class parser
{
public:
	//文法分析
	vector<treeNode*> SyntacticAnalyse(vector<token> tokenVec, bool & isError);
	parser();
	~parser();


private:
	int index;
	string errorStr;
	vector<treeNode*> m_treeNodeVec;
	vector<token> m_tokenVec;
	treeNode* m_errorNode;
	token m_currentToken;
	vector<queue<treeNodeString> > m_queueVec;
	//int m_paraIndex;

	treeNode* parseStmt();								//段落
	treeNode* parseIfStmt();							//if语句
	treeNode* parseWhileStmt();							//while语句
	treeNode* parseForStmt();							//for语句
	treeNode* parseReadStmt();							//read语句
	treeNode* parseWriteStmt();							//write语句
	treeNode* parseDeclareStmt(bool IsInsideFunction);	//声明语句
	treeNode* parseAssignStmt(bool IsInsideFunction);	//赋值语句
	treeNode* parseStmtBlock();							//语句块
	treeNode* parseExp();								//表达式
	treeNode* parsePolynomial();						//多项式
	treeNode* parseTerm();								//项
	treeNode* parseFactor();							//因子
	treeNode* pareseLiteral();							//字面值
	treeNode* parseLogicOp();							//逻辑运算符
	treeNode* parseAddMinOp();							//+ -
	treeNode* parseMutDivOp();							//* /
	treeNode* parseVariableName();						//变量名
	treeNode* parseFunctionDeclare();					//函数声明
	treeNode* pareseFunctionCall();						//函数调用
	treeNode* parseVariableType();						//变量类型
	treeNode* parseReturn();							//return语句


	void ConsumeNextToken(int type);		//消耗下一个token
	int GetNextTokenType();					//获得下一个token类型
	int GetNextNextTokenType();				//获得下下一个token类型
	int GetNextTokenLineNo();				//获得下一个token行号
	int GetNextTokenColumnNo();				//获得下一个token列号
};

