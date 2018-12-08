#include<iostream>
#include "parser.h"
#include "lexer.h"
#include<vector>
#include<fstream>
#include "token.h"
#include "error.h"
#include "treeNode.h"
#include "codegenerater.h"
#include "quarterExp.h"
#include "execute.h"
using namespace std;

int main() {
	lexer* lex = new lexer();
	parser* parse = new parser();
	ifstream myfile("cmm3.cmm");
	string temp, total = "";
	if (!myfile.is_open()) {
		cout << "δ�ɹ����ļ�" << endl;
	}
	while (getline(myfile, temp)) {
		total += temp +"\n";
	}
	vector<token> tokenVec = lex->LexAnalyze(total);
	cout << "�ʷ�����:" << endl;
	for (int i = 0; i < tokenVec.size(); i++) {
		cout << " �к�: " << tokenVec[i].toString() << endl;
	}
	string errorStr;
	bool isParseError;
	vector<treeNode*> treeNodeVec = parse->SyntacticAnalyse(tokenVec,isParseError);
	if (!isParseError) {
		cout << "�﷨����:" << endl;
		for (int i = 0; i < treeNodeVec.size(); i++) {
			treeNode::travelTree(treeNodeVec[i], 0);
		}
		codegenerater* cg = new codegenerater();
		bool isGeneraterError;
		vector<quarterExp> midCode = cg->generateCode(treeNodeVec, errorStr, isGeneraterError);
		if (!isGeneraterError) {
			cout << "�������:" << endl;
			for (int i = 0; i < midCode.size(); i++) {
			cout << midCode[i].toString() << endl;
			}
			execute* exe = new execute();
			cout << "���н��Ϊ:" << endl;
			bool isExecute = exe->init(midCode);
			if(isExecute)
				exe->run();
		}
		else {
			cout << "�������ʧ�ܣ�" << endl;
			cout << errorStr;
		}
	}
	system("pause");
	return 0;
}
