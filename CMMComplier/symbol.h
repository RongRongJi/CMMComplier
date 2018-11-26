#pragma once
#include<iostream>
#include<string>
using namespace std;
class symbol
{
public:
	enum {
		TEMP,
		REG,
		SINGLE_INT,
		SINGLE_REAL,
		SINGLE_CHAR
	};

	symbol();
	~symbol();
	symbol(const string & name, int type = TEMP);
	symbol(int valueIndex, int type = REG);
	symbol(const string & name, int type, int lineNo, int level, int elementNum = 0);

	int type();
	void setType(int type);
	int dataSize();
	void setDataSize(int dataSize);
	int level();
	void setLeve(int level);
	string name();
	void setName(string & name);
	int elementNum();
	void setElementNum(int elementNum);
	int lineNo();
	void setLineNo(int lineNo);
	int valueIndex();
	void setValueIndex(int valueIndex);
	
private:
	string m_name;
	int m_lineNo;
	int m_type;
	int m_dataSize;
	int m_level;
	int m_elementNum;
	int m_valueIndex;

};

