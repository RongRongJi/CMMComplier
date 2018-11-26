#pragma once
#include "symbol.h"
#include "funsymbol.h"
#include "treeNode.h"
#include <vector>
#include<string>
#include<iostream>
using namespace std;
class symbolTable
{
public:
	symbolTable();
	~symbolTable();
	void clearVec();
	void registerSymbol(symbol  symbol);
	void registerFunSymbol(funsymbol  funSymbol);
	void deregisterSymbol(int level);

	string getNewTempSymbolName();
	symbol getSymbol(string  name, int  index, int  dereference);
	int getSymbolType(string  name);
	int getCurFunSymbolType();
	funsymbol getFunSymbol(string  name, int lineNo);
	int getLastSymbolType();
	void clearTempVec();
	int checkSymbolIsDeclared(treeNode * node, int lineNo);

	vector<symbol> symbolVec() ;
	void setSymbolVec(std::vector<symbol> &symbolVec);

	vector<funsymbol> funSymbolVec();
	void setFunSymbolVec(vector<funsymbol> &funSymbolVec);

private:
	static const string TEMP_PREFIX;

	vector<symbol> m_symbolVec;
	vector<funsymbol> m_funSymbolVec;
	vector<symbol> m_tempSymbolVec;
};

