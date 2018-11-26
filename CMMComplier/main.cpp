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
	ifstream myfile("hello.cmm");
	//ofstream outfile("hello.par");
	string temp, total = "";
	if (!myfile.is_open()) {
		cout << "未成功打开文件" << endl;
	}
	while (getline(myfile, temp)) {
		total += temp + "\n";
	}
	vector<token> tokenVec = lex->LexAnalyze(total);
	string errorStr;
	bool isParseSucceed;
	vector<treeNode*> treeNodeVec = parse->SyntacticAnalyse(tokenVec,isParseSucceed);
	for (int i = 0; i < treeNodeVec.size(); i++) {
		treeNode::travelTree(treeNodeVec[i], 0);
	}
	codegenerater* cg = new codegenerater();
	vector<quarterExp> midCode = cg->generateCode(treeNodeVec, errorStr);
	for (int i = 0; i < midCode.size(); i++) {
		cout << midCode[i].toString() << endl;
	}
	execute* exe = new execute();
	cout << "运行结果为:" << endl;
	exe->init(midCode);
	exe->run();
	cout << endl;
	system("pause");
	return 0;
}
