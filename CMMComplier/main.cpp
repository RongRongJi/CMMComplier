#include"lexer.h"
#include"error.h"
#include"token.h"
#include<vector>
#include<iostream>
#include<fstream>
using namespace std;
int main() {
	lexer* lex = new lexer();
	ifstream myfile("hello.cmm");
	ofstream outfile("hello.lex");
	string temp;
	if (!myfile.is_open()) {
		cout << "未成功打开文件" << endl;
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