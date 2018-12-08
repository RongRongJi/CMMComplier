#pragma once
#include "quarterExp.h"
#include <vector>
#include "symbolTable.h"
#include "value.h"
#include <map>
#include <stack>
#include <sstream>
#include <iostream>
using namespace std;
class execute
{
public:
	enum {
		RUN,
		SINGLE_STMT,
		SINGLE_PRO
	};
	execute();
	~execute();
	bool init(vector<quarterExp> qeVec);
	void runSingleStmt();
	void inputToStream(string value);
	
	bool isOver();
	void setIsOver(bool isOver);

	void run();
	
private:
	static const int DATA_ADDR = 0x50000000;    //数据段起始地址(5->4)
	static const int GLOBAL_ADDR = 0x30000000;  //全局区起始地址(3->4)
	static const int CONST_ADDR = 0x20000000;   //常量区起始地址(2->3)
	static const int CODE_ADDR = 0x10000000;    //代码段起始地址(1->2)
	int m_nextQeVecIndex;						//四元式行数
	int m_ebp;									//栈底
	int m_esp;									//栈顶
	stack<int> m_previousEbp;					//前栈底
	stack<int> m_previousEsp;					//前栈顶
	int m_level;								//代码层数
	vector<value> m_valueVec;					//所有数值
	map<int, int> m_addrMap;					//地址
	int m_curDataAddr;							//当前地址
	symbolTable m_symbolTable;					//符号表
	vector<quarterExp> m_qeVec;					//四元式表
	map<string, value> m_tempMap;				//临时变量
	vector<value> m_constVec;					//常量集
	map<int, int> m_constMap;					//常量map
	int m_curConstAddr;							//当前常量地址
	bool m_isOver;								//是否结束
	vector<value> m_globalVec;					//全局集
	map<int, int> m_globalMap;					//全局对
	int m_curGlobalAddr;						//当前全局地址
	int m_executeStmtLineNo;					//正在执行的行数

	stringstream m_inputStream;					//输入流


private:
	int getStringType(string str);	//获取四元式类型
	value readValue(string str);	//根据字符串读值
	void assignProcess(symbol sym, string str);		//赋值操作
	//赋值辅助操作
	void assignUtil(int valueIndex, int index, string valueStr, int valueType, bool isGlobal);
	void declareProcess(quarterExp qExp, string type);		//声明操作
	int getValueIndex(string addrValue, int &addrRange, int &offset);	//根据地址锁定位置
	int getRegNum(string str);		//获取栈顶或栈底的值
};

