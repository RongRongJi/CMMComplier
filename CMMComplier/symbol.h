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
	string m_name;			//符号名
	int m_lineNo;			//行号
	int m_type;				//符号类型
	int m_dataSize;			//符号所占空间
	int m_level;			//符号所在层数
	int m_elementNum;		//元素数量
	int m_valueIndex;		//当前指数

};

