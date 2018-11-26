#pragma once
#include<string>
#include<vector>
#include<iostream>
using namespace std;
class funsymbol
{
public:
	enum {
		INT,
		REAL,
		CHAR,
		VOID
	};
	funsymbol();
	~funsymbol();
	funsymbol(const std::string & name, int lineNo, int returnType, int stmtIndex = -1);

	std::string name() ;
	void setName(const std::string &name);

	int lineNo() ;
	void setLineNo(int lineNo);

	int returnType() ;
	void setReturnType(int returnType);

	std::vector<int> m_argTypeVec;

	int stmtIndex() ;
	void setStmtIndex(int stmtIndex);

private:
	std::string m_name;
	int m_lineNo;
	int m_returnType;
	int m_stmtIndex;
};

