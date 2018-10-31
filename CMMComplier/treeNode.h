#pragma once
#include<iostream>
#include<vector>
#include<string>
using namespace std;
class treeNode
{
public:
	enum {
		NUL,            //��
		IF_STMT,        //if���
		WHILE_STMT,     //while���
		FOR_STMT,       //for���
		READ_STMT,      //read���
		WRITE_STMT,     //write���
		DECLARE_STMT,   //�������
		ASSIGN_STMT,    //��ֵ���
		STMT_BLOCK,		//{ }���
		RETURN_STMT,    //return���
		EXP,            //���ʽ
		ADDTIVE_EXP,	//������ʽ
		TERM,			//��
		VAR,            //����
		FUNDECLARE,     //��������
		FUNCALL,        //��������
		VARTYPE,        //��������
		OP,             //�����
		FACTOR,         //����
		LITERAL,        //����ֵ
	};
	treeNode(int type);
	~treeNode();
	treeNode * getLeft();
	void setLeft(treeNode *left);

	treeNode * getMiddle();
	void setMiddle(treeNode *middle);

	treeNode * getRight();
	void setRight(treeNode *right);

	treeNode * getNext();
	void setNext(treeNode *next);

	int getType();
	void setType(int type);

	int getDataType();
	void setDataType(int dataType);

	std::string getValue();
	void setValue(const std::string &value);

	int getLineNo();
	void setLineNo(int lineNo);

	string toString();
	static void travelTree(treeNode * node,int blk);

private:
	int m_type;
	int m_lineNo;
	std::vector<treeNode *> m_childVec;
	treeNode * m_left;
	treeNode * m_middle;
	treeNode * m_right;
	treeNode * m_next;    //����Ǵ�����еĴ���,��m_nextָ��������һ�����

	int m_dataType;

	
	string m_value;
};

