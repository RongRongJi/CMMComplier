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
		cout << "未成功打开文件" << endl;
	}
	while (getline(myfile, temp)) {
		total += temp +"\n";
	}
	vector<token> tokenVec = lex->LexAnalyze(total);
	cout << "词法分析:" << endl;
	for (int i = 0; i < tokenVec.size(); i++) {
		cout << " 行号: " << tokenVec[i].toString() << endl;
	}
	string errorStr;
	bool isParseError;
	vector<treeNode*> treeNodeVec = parse->SyntacticAnalyse(tokenVec,isParseError);
	if (!isParseError) {
		cout << "语法分析:" << endl;
		for (int i = 0; i < treeNodeVec.size(); i++) {
			treeNode::travelTree(treeNodeVec[i], 0);
		}
		codegenerater* cg = new codegenerater();
		bool isGeneraterError;
		vector<quarterExp> midCode = cg->generateCode(treeNodeVec, errorStr, isGeneraterError);
		if (!isGeneraterError) {
			cout << "语义分析:" << endl;
			for (int i = 0; i < midCode.size(); i++) {
			cout << midCode[i].toString() << endl;
			}
			execute* exe = new execute();
			cout << "运行结果为:" << endl;
			bool isExecute = exe->init(midCode);
			if(isExecute)
				exe->run();
		}
		else {
			cout << "语义分析失败！" << endl;
			cout << errorStr;
		}
	}
	system("pause");
	return 0;
}
