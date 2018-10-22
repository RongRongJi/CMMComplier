#pragma once
#include<vector>
#include<string>
#include<sstream>
#include "token.h"
#include "error.h"
using namespace std;
class lexer
{
public:
	lexer();
	vector<token> LexAnalyze(const string & text);//�ʷ�����
	~lexer();
private:
	void readChar();		//��ȡһ���ַ�
	bool isBlank();			//�ж��Ƿ�Ϊ�հ�
	bool isSingle();		//�ж��Ƿ��ǵ�������
	bool isCombine();		//�ж��Ƿ�Ϊ���Ϸ���
	bool isDigit();			//�ж��Ƿ�Ϊ����
	bool isReserveWordOrID();	//�ж��Ƿ��Ǳ����ֻ��ʶ��
	void errorHandle(stringstream& stream);		//������

	bool m_isEOF;			//�Ƿ��ȡ����
	int m_lineNo;			//�к�
	int m_columnNo;			//�к�
	vector<token> m_tokenVec;	//���token
	char m_currentChar;		//��ǰ��ȡ���ַ�
	stringstream m_stream;	//������
};

