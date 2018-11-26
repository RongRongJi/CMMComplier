#pragma once
#include<string>
using namespace std;
class quarterExp
{
public:
	static  string JMP;
	static  string READ;
	static  string WRITE;
	static  string IN;
	static  string OUT;
	static  string INT;
	static  string REAL;
	static  string CHAR;
	static  string ASSIGN;
	static  string PLUS;
	static  string MINUS;
	static  string MUL;
	static  string DIV;
	static  string GT;
	static  string LT;
	static  string GET;
	static  string LET;
	static  string EQ;
	static  string NEQ;
	static  string CALL;
	static  string CALLFH;
	static  string VOID;
	quarterExp();
	~quarterExp();
	quarterExp(string first, string second, string  third, string  fourth, int lineNo);

	string first();
	void setFirst(string  first);

	string second();
	void setSecond(string  second);

	string third();
	void setThird(string  third);

	string fourth();
	void setFourth(string  fourth);

	static int index();
	static void setIndex(int index);

	int lineNo();
	void setLineNo(int lineNo);

	string toString();

private:
	string m_first;
	string m_second;
	string m_third;
	string m_fourth;
	int m_lineNo;
	static int m_index;
};

