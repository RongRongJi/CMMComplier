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
	static const int DATA_ADDR = 0x50000000;    //���ݶ���ʼ��ַ(5->4)
	static const int GLOBAL_ADDR = 0x30000000;  //ȫ������ʼ��ַ(3->4)
	static const int CONST_ADDR = 0x20000000;   //��������ʼ��ַ(2->3)
	static const int CODE_ADDR = 0x10000000;    //�������ʼ��ַ(1->2)
	int m_nextQeVecIndex;						//��Ԫʽ����
	int m_ebp;									//ջ��
	int m_esp;									//ջ��
	stack<int> m_previousEbp;					//ǰջ��
	stack<int> m_previousEsp;					//ǰջ��
	int m_level;								//�������
	vector<value> m_valueVec;					//������ֵ
	map<int, int> m_addrMap;					//��ַ
	int m_curDataAddr;							//��ǰ��ַ
	symbolTable m_symbolTable;					//���ű�
	vector<quarterExp> m_qeVec;					//��Ԫʽ��
	map<string, value> m_tempMap;				//��ʱ����
	vector<value> m_constVec;					//������
	map<int, int> m_constMap;					//����map
	int m_curConstAddr;							//��ǰ������ַ
	bool m_isOver;								//�Ƿ����
	vector<value> m_globalVec;					//ȫ�ּ�
	map<int, int> m_globalMap;					//ȫ�ֶ�
	int m_curGlobalAddr;						//��ǰȫ�ֵ�ַ
	int m_executeStmtLineNo;					//����ִ�е�����

	stringstream m_inputStream;					//������


private:
	int getStringType(string str);	//��ȡ��Ԫʽ����
	value readValue(string str);	//�����ַ�����ֵ
	void assignProcess(symbol sym, string str);		//��ֵ����
	//��ֵ��������
	void assignUtil(int valueIndex, int index, string valueStr, int valueType, bool isGlobal);
	void declareProcess(quarterExp qExp, string type);		//��������
	int getValueIndex(string addrValue, int &addrRange, int &offset);	//���ݵ�ַ����λ��
	int getRegNum(string str);		//��ȡջ����ջ�׵�ֵ
};

