#pragma once
#include<iostream>
#include<vector>
#include<string>
using namespace std;
class treeNode
{
public:
	enum {
		NUL,            //空
		IF_STMT,        //if语句
		WHILE_STMT,     //while语句
		FOR_STMT,       //for语句
		READ_STMT,      //read语句
		WRITE_STMT,     //write语句
		DECLARE_STMT,   //声明语句
		ASSIGN_STMT,    //赋值语句
		STMT_BLOCK,		//{ }语句
		RETURN_STMT,    //return语句
		EXP,            //表达式
		ADDTIVE_EXP,	//多项表达式
		TERM,			//项
		VAR,            //变量
		FUNDECLARE,     //函数声明
		FUNCALL,        //函数调用
		VARTYPE,        //变量类型
		OP,             //运算符
		FACTOR,         //因子
		LITERAL,        //字面值
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
	treeNode * m_next;    //如果是代码块中的代码,则m_next指向其后面的一条语句

	int m_dataType;

	
	string m_value;
};

