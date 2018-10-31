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
	//ÎÄ·¨·ÖÎö
	vector<treeNode*> SyntacticAnalyse(vector<token> tokenVec);
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
	int m_paraIndex;
	treeNode* parseStmt();
	treeNode* parseIfStmt();
	treeNode* parseWhileStmt();
	treeNode* parseForStmt();
	treeNode* parseReadStmt();
	treeNode* parseWriteStmt();
	treeNode* parseDeclareStmt(bool IsInsideFunction);
	treeNode* parseAssignStmt(bool IsInsideFunction);
	treeNode* parseStmtBlock();
	treeNode* parseExp();
	treeNode* parsePolynomial();
	treeNode* parseTerm();
	treeNode* parseFactor();
	treeNode* pareseLiteral();
	treeNode* parseLogicOp();
	treeNode* parseAddMinOp();
	treeNode* parseMutDivOp();
	treeNode* parseVariableName();
	treeNode* parseFunctionDeclare();
	treeNode* pareseFunctionCall();
	treeNode* parseVariableType();
	treeNode* parseReturn();


	void ConsumeNextToken(int type);
	int GetNextTokenType();
	int GetNextNextTokenType();
	int GetNextTokenLineNo();
	int GetNextTokenColumnNo();
};

