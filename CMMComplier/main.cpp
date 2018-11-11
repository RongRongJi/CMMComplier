#include<iostream>
#include "parser.h"
#include "lexer.h"
#include<vector>
#include<fstream>
#include "token.h"
#include "error.h"
#include "treeNode.h"
using namespace std;
int Lexer() {
	lexer* lex = new lexer();
	ifstream myfile("hello.cmm");
	ofstream outfile("hello.lex");
	string temp;
	if (!myfile.is_open()) {
		cout << "δ�ɹ����ļ�" << endl;
	}
	int nLineNo = 1;
	string strAll = "";
	while (getline(myfile, temp)) {
		strAll = strAll + temp + "\n";
	}
	myfile.close();
	vector<token> tmps = lex->LexAnalyze(strAll);
	for (int i = 0; i < tmps.size(); i++) {
		outfile << tmps[i].toString() << endl;
	}
	outfile.close();
	return 0;
}
int Parser() {
	lexer* lex = new lexer();
	parser* parse = new parser();
	ifstream myfile("hello.cmm");
	string temp, total = "";
	if (!myfile.is_open()) {
		cout << "δ�ɹ����ļ�" << endl;
	}
	while (getline(myfile, temp)) {
		total += temp + "\n";
	}
	vector<token> tokenVec = lex->LexAnalyze(total);
	/*cout << "�ʷ�����" << endl;
	for (int i = 0; i < tokenVec.size(); i++) {
		cout << tokenVec[i].toString() << endl;
	}*/
	string errorStr;
	bool isError = false;
	vector<treeNode*> treeNodeVec = parse->SyntacticAnalyse(tokenVec,isError);
	if (!isError) {
		cout << "�﷨����" << endl;
		for (int i = 0; i < treeNodeVec.size(); i++) {
			treeNode::travelTree(treeNodeVec[i], 1);
			cout << endl;
		}
	}
	system("pause");
	return 0;
}
int main() {
	//�ʷ�����
	//Lexer();
	//�﷨����
	Parser();
	return 0;
}
