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
	//�ķ�����
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

	treeNode* parseStmt();								//����
	treeNode* parseIfStmt();							//if���
	treeNode* parseWhileStmt();							//while���
	treeNode* parseForStmt();							//for���
	treeNode* parseReadStmt();							//read���
	treeNode* parseWriteStmt();							//write���
	treeNode* parseDeclareStmt(bool IsInsideFunction);	//�������
	treeNode* parseAssignStmt(bool IsInsideFunction);	//��ֵ���
	treeNode* parseStmtBlock();							//����
	treeNode* parseExp();								//���ʽ
	treeNode* parsePolynomial();						//����ʽ
	treeNode* parseTerm();								//��
	treeNode* parseFactor();							//����
	treeNode* pareseLiteral();							//����ֵ
	treeNode* parseLogicOp();							//�߼������
	treeNode* parseAddMinOp();							//+ -
	treeNode* parseMutDivOp();							//* /
	treeNode* parseVariableName();						//������
	treeNode* parseFunctionDeclare();					//��������
	treeNode* pareseFunctionCall();						//��������
	treeNode* parseVariableType();						//��������
	treeNode* parseReturn();							//return���


	void ConsumeNextToken(int type);		//������һ��token
	int GetNextTokenType();					//�����һ��token����
	int GetNextNextTokenType();				//�������һ��token����
	int GetNextTokenLineNo();				//�����һ��token�к�
	int GetNextTokenColumnNo();				//�����һ��token�к�
};

