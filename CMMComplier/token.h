#pragma once
#include<string>
using namespace std;
class token
{
public:
	//���ű�
	enum {
		//��
		NULLL,
		//������
		ERR,
		//������
		IF,
		ELSE,
		WHILE,
		FOR,
		READ,
		WRITE,
		INT,
		REAL,
		CHAR,
		VOI,
		//�������
		PLUS,//+
		MINUS,//-
		MUL,//*
		DIV,// /
		ASSIGN,// =
		LT,// <
		GT,// >
		EQ,// ==
		NEQ,// <>
		LPS,// (
		RPS,// )
		SEMI,// ;
		LBRACE,//{
		RBRACE,// }
		LCOM,// /*
		RCOM,// */
		SCOM,// //
		LBRACKET,// [
		RBRACKET,// ]
		SINQS, // \'
		DOUQS, // \"
		LET,// <=
		GET,// >=
		//��ʶ��
		ID,
		RETURN,
		//ʮ���Ƶ�������ʵ��
		INT_VALUE,
		REAL_VALUE,
		CHAR_VALUE,
		STR_VALUE

	};
	token();
	token(int type, int lineNo, int columnNo, string value = "");
	virtual ~token();
	string toString();				//��token�����Ϣ���
	int lineNoValue();				//�к�
	int columnNoValue();			//�к�
	int typeValue();				//token���ͱ��
	string value() const;			//token����
	static string typeToString(int type);	//token���ͱ�ŷ��������
private:
	int m_type;				//���ͺ�
	int m_lineNo;			//�к�
	int m_columnNo;			//�к�
	string m_value;			//����
};

