#include "symbolTable.h"
#include "codegeneraterException.h"
#include<sstream>

const string symbolTable::TEMP_PREFIX = "#T";

symbolTable::symbolTable()
{
}


symbolTable::~symbolTable()
{
}

void symbolTable::clearVec()
{
	m_symbolVec.clear();
	m_tempSymbolVec.clear();
	m_funSymbolVec.clear();
}

void symbolTable::registerSymbol(symbol  symbol)
{
	for (int i = 0; i < m_symbolVec.size(); i++) {
		if (m_symbolVec[i].name() == symbol.name() && m_symbolVec[i].level() == symbol.level()) {
			stringstream ss;
			ss << "'" << symbol.name() << "' 已经被声明过";
			throw codegeneraterException(symbol.lineNo(), ss.str());
		}
	}
	m_symbolVec.push_back(symbol);
}

void symbolTable::registerFunSymbol(funsymbol  funSymbol)
{
	for (int i = 0; i < m_funSymbolVec.size(); i++) {
		if (m_funSymbolVec[i].name() == funSymbol.name()) {
			stringstream ss;
			ss << "'" << funSymbol.name() << "'函数已经被声明过";
			throw codegeneraterException(funSymbol.lineNo(), ss.str());
		}
	}
	m_funSymbolVec.push_back(funSymbol);
}

void symbolTable::deregisterSymbol(int level)
{
	while (!m_symbolVec.empty() && m_symbolVec.back().level() == level)
		m_symbolVec.pop_back();
}

string symbolTable::getNewTempSymbolName()
{
	string temp = "";
	for (int i = 1;; i++) {
		stringstream ss;
		ss << TEMP_PREFIX << i;
		temp = ss.str();
		bool isExisted = false;
		for (int j = 0; j < m_tempSymbolVec.size(); j++) {
			if (m_tempSymbolVec[j].name() == temp) {
				isExisted = true;
				break;
			}
		}
		if (isExisted)
			continue;
		symbol s(temp, symbol::TEMP, -1, -1);
		m_tempSymbolVec.push_back(s);
		return s.name();
	}
}

symbol symbolTable::getSymbol(string  name)
{
	for (int i = m_symbolVec.size() - 1; i >= 0; i--) {
		symbol tempSymbol = m_symbolVec[i];
		if (tempSymbol.name() == name) {
			return tempSymbol;
		}
	}
	if (name[0] == '#')
		return symbol(name);
}

int symbolTable::getSymbolType(string  name)
{
	return getSymbol(name).type();
}

int symbolTable::getCurFunSymbolType()
{
	return m_funSymbolVec.back().returnType();
}

funsymbol symbolTable::getFunSymbol(string  name, int lineNo)
{
	for (int i = 0; i < m_funSymbolVec.size(); i++) {
		funsymbol funSymbol = m_funSymbolVec[i];
		if (funSymbol.name() == name)
			return funSymbol;
	}
	throw codegeneraterException(lineNo, "该函数未声明");
}

int symbolTable::getLastSymbolType()
{
	return m_symbolVec.back().type();
}

void symbolTable::clearTempVec()
{
	m_tempSymbolVec.clear();
}

int symbolTable::checkSymbolIsDeclared(treeNode * node, int lineNo)
{
	for (int i = 0; i < m_symbolVec.size(); i++) {
		symbol tempSymbol = m_symbolVec[i];
		if (tempSymbol.name() == node->getValue()) {
			if (tempSymbol.elementNum() == 0) {
				if (node->getLeft() == NULL)
					return 0;  //普通变量
			}
			else if (tempSymbol.elementNum() < 0) {
				if (node->getLeft() == NULL)
					return -1; 
				else
					return 0;  //普通变量
			}
		}
	}
}

vector<symbol> symbolTable::symbolVec()
{
	return m_symbolVec;
}

void symbolTable::setSymbolVec(std::vector<symbol>& symbolVec)
{
	m_symbolVec = symbolVec;
}

vector<funsymbol> symbolTable::funSymbolVec()
{
	return m_funSymbolVec;
}

void symbolTable::setFunSymbolVec(vector<funsymbol>& funSymbolVec)
{
	m_funSymbolVec = funSymbolVec;
}
