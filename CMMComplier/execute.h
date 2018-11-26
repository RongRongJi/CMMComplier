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
	int runMode();
	void setRunMode(int runMode);
	
	bool isOver();
	void setIsOver(bool isOver);

	int isRunFinished();
	void setIsRunFinished(int isRunFinished);
	int isExceptionHappened();
	void setIsExceptionHappened(int isExceptionHappened);
	void run();
	
private:
	static const int DATA_ADDR = 0x50000000;    //数据段起始地址(5->4)
	static const int GLOBAL_ADDR = 0x30000000;  //全局区起始地址(3->4)
	static const int CONST_ADDR = 0x20000000;   //常量区起始地址(2->3)
	static const int CODE_ADDR = 0x10000000;    //代码段起始地址(1->2)
	int m_nextQeVecIndex;
	int m_ebp;
	int m_esp;
	stack<int> m_previousEbp;
	stack<int> m_previousEsp;
	int m_level;
	bool m_waitInput;
	vector<value> m_valueVec;
	map<int, int> m_addrMap;
	int m_curDataAddr;
	symbolTable m_symbolTable;
	vector<quarterExp> m_qeVec;
	map<string, value> m_tempMap;
	vector<value> m_constVec;
	map<int, int> m_constMap;
	int m_curConstAddr;
	bool m_isOver;
	vector<value> m_globalVec;
	map<int, int> m_globalMap;
	int m_curGlobalAddr;
	int m_executeStmtLineNo;

	stringstream m_inputStream;

	int m_isRunFinished;
	int m_isExceptionHappened;

private:
	int getStringType(string str);
	value readValue(string str);
	void assignProcess(symbol sym, int index, int dereference, string str);
	void assignUtil(int valueIndex, int index, string valueStr, int valueType, bool isGlobal);
	void declareProcess(quarterExp qExp, string type);
	int getValueIndex(string addrValue, int &addrRange, int &offset);
	int getScript(string varStr);
	int getRegNum(string str);
	string addrFromStr(string addr);
	string addrFromInt(int addr);
	void setAddrUnitSize(int symbolType);
	void setAddrUnitSize(string addrStr);
};

