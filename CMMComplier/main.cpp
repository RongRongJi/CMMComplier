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
	ifstream myfile("cmm4.cmm");
	//ofstream outfile("hello.par");
	string temp, total = "";
	if (!myfile.is_open()) {
		cout << "δ�ɹ����ļ�" << endl;
	}
	while (getline(myfile, temp)) {
		total += temp + "\n";
	}
	vector<token> tokenVec = lex->LexAnalyze(total);
	string errorStr;
	bool isParseError;
	vector<treeNode*> treeNodeVec = parse->SyntacticAnalyse(tokenVec,isParseError);
	if (!isParseError) {
		/*for (int i = 0; i < treeNodeVec.size(); i++) {
		treeNode::travelTree(treeNodeVec[i], 0);
		}*/
		codegenerater* cg = new codegenerater();
		bool isGeneraterError;
		vector<quarterExp> midCode = cg->generateCode(treeNodeVec, errorStr, isGeneraterError);
		if (!isGeneraterError) {
			/*for (int i = 0; i < midCode.size(); i++) {
			cout << midCode[i].toString() << endl;
			}*/
			execute* exe = new execute();
			cout << "���н��Ϊ:" << endl;
			exe->init(midCode);
			exe->run();
		}
		else {
			cout << errorStr;
		}
	}
	system("pause");
	return 0;
}
