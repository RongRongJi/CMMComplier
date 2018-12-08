#include "execute.h"
#include "common.h"
#include "codegenerater.h"
#include "executeException.h"

execute::execute()
{
}


execute::~execute()
{
}


bool execute::init(vector<quarterExp> qeVec)
{
	codegenerater * cg = new codegenerater();
	m_nextQeVecIndex = cg->getEntry();
	m_ebp = 0;
	m_esp = 1;
	while (!m_previousEbp.empty())
		m_previousEbp.pop();
	while (!m_previousEsp.empty())
		m_previousEsp.pop();
	m_level = 0;
	m_valueVec.clear();
	m_valueVec.push_back(value("$" + itos(0x00000000), value::ADDR));
	m_addrMap.clear();
	m_addrMap[0] = 0x50000000;
	m_curDataAddr = 0x50000000;
	m_symbolTable.clearVec();
	m_qeVec = qeVec;
	m_tempMap.clear();
	m_constVec.clear();
	m_constMap.clear();
	m_curConstAddr = CONST_ADDR;
	m_globalVec.clear();
	m_globalMap.clear();
	m_isOver = false;
	try {
		if (m_nextQeVecIndex < 0)
			throw executeException(-1, "无法找到main函数入口");
		for (int i = 0; i < globalVarVec.size(); i++) {
			m_nextQeVecIndex = globalVarVec[i];
			runSingleStmt();
		}
		m_nextQeVecIndex = cg->getEntry();
	}
	catch (executeException e) {
		cout << e.message();
		return false;
	}
	return true;
}

/*
四元式执行：
JMP:
第二元进行判断，如果非0则执行JMP，跳转到第四元所指向的四元式
READ:
根据第四元的变量类型，声明该类型的变量，并调用cin输入值
WRITE:
根据第四元的变量的值存入returnValue，调用cout输出returnValue
IN:
将旧栈顶存入栈集中，增加层数
OUT:
减少层数，撤销该层的所有符号表，将旧栈顶弹出
INT/REAL/CHAR/VOID:
通过符号表查询所需的变量长度，申请空间
ASSIGN:
将第二元的值赋值给第四元的变量
PLUS/MINUS/MUL/DIV/GT/GET/LT/LET/EQ/NEQ:
将第二元第三元的值进行第一元所指的运算操作，并把结果保存在第四元的临时变量中
CALL:
将旧栈底存入栈集中，新的栈底为栈顶-1
CALLFH:
新栈顶为原栈底，新栈底从栈集中弹出
*/
void execute::runSingleStmt()
{
	quarterExp qExp = m_qeVec[m_nextQeVecIndex];
	m_executeStmtLineNo = qExp.lineNo();
	m_nextQeVecIndex++;
	string type = qExp.first();
	if (type == quarterExp::JMP) {
		if (qExp.second() == "" || readValue(qExp.second()).valueStr() == "0") {
			value jupValue = readValue(qExp.fourth());
			if (jupValue.type() == value::ADDR) {
				if (jupValue.valueStr() == "$" + itos(0x00000000))
					m_isOver = true;
				int addrRange, offset;
				addrRange = offset = 0;
				m_nextQeVecIndex = getValueIndex(jupValue.valueStr(), addrRange, offset);
			}
			else {
				m_nextQeVecIndex = atoi(jupValue.valueStr().c_str());
			}
		}
	}
	else if (type == quarterExp::READ) {
		symbol tempSymbol = m_symbolTable.getSymbol(qExp.fourth());
		string tempValue;
		cin >> tempValue;
		inputToStream(tempValue);
		switch (tempSymbol.type()) {
		case symbol::SINGLE_INT:
			int intTemp;
			m_inputStream >> intTemp;
			assignProcess(tempSymbol, itos(intTemp));
			break;
		case symbol::SINGLE_REAL:
			double doubleTemp;
			m_inputStream >> doubleTemp;
			assignProcess(tempSymbol, dtos(doubleTemp));
			break;
		case symbol::SINGLE_CHAR:
			char charTemp;
			m_inputStream >> charTemp;
			assignProcess(tempSymbol, "'" + ctos(charTemp) + "'");
			break;
		default:
			break;
		}
	}
	else if (type == quarterExp::WRITE) {
		value tempValue = readValue(qExp.fourth());
		string returnValue = "";
		if (tempValue.type() == value::CHAR_VALUE) {
			string charStr = tempValue.valueStr();
			if (charStr.size() <= 2)
				returnValue = "";
			else
				returnValue = ctos(tempValue.valueStr()[1]);
		}
		else if (tempValue.type() == value::ADDR) {
			int addrRange = 0;
			int offset = 0;
			int addrIndex = getValueIndex(tempValue.valueStr(), addrRange, offset);
			if (addrRange == 1) {
				while (addrIndex > 0 && m_valueVec[addrIndex].type() == value::CHAR_VALUE
					&& m_valueVec[addrIndex].valueStr() != "''")
				{
					returnValue += ctos(m_valueVec[addrIndex].valueStr()[1]);
					addrIndex--;
				}
			}
			else if (addrRange == 2) {
				while (addrIndex <m_globalVec.size() &&
					m_globalVec[addrIndex].type() == value::CHAR_VALUE
					&& m_globalVec[addrIndex].valueStr() != "''")
				{
					returnValue += ctos(m_globalVec[addrIndex].valueStr()[1]);
					addrIndex++;
				}
			}
			else if (addrRange == 3) {
				while (addrIndex < m_constVec.size()
					&& m_constVec[addrIndex].type() == value::CHAR_VALUE
					&& m_constVec[addrIndex].valueStr() != "''")
				{
					returnValue += ctos(m_constVec[addrIndex].valueStr()[1]);
					addrIndex++;
				}
			}
		}
		else {
			returnValue = tempValue.valueStr();
		}
		cout << returnValue;
	}
	else if (type == quarterExp::IN) {
		m_previousEsp.push(m_esp);
		m_level++;
	}
	else if (type == quarterExp::OUT) {
		m_symbolTable.deregisterSymbol(m_level);
		m_level--;
		int tempEsp = m_previousEsp.top();
		m_previousEsp.pop();
		m_valueVec.erase(m_valueVec.begin() + tempEsp, m_valueVec.end());
		m_esp = tempEsp;
	}
	else if (type == quarterExp::INT) {
		declareProcess(qExp, quarterExp::INT);
	}
	else if (type == quarterExp::REAL) {
		declareProcess(qExp, quarterExp::REAL);
	}
	else if (type == quarterExp::CHAR) {
		declareProcess(qExp, quarterExp::CHAR);
	}
	else if (type == quarterExp::VOID) {
		declareProcess(qExp, quarterExp::VOID);
	}
	else if (type == quarterExp::ASSIGN) {
		symbol tempSymbol;
		if (qExp.fourth()[0] == '@') {
			tempSymbol = symbol(getRegNum(qExp.fourth()));
		}
		else {
			tempSymbol = m_symbolTable.getSymbol(qExp.fourth());
		}
		assignProcess(tempSymbol, qExp.second());
	}
	else if (type == quarterExp::PLUS) {
		symbol tempSymbol = m_symbolTable.getSymbol(qExp.fourth());
		value l_value = readValue(qExp.second());
		value r_value = readValue(qExp.third());
		string result = l_value + r_value;
		assignProcess(tempSymbol, result);
	}
	else if (type == quarterExp::MINUS) {
		symbol tempSymbol = m_symbolTable.getSymbol(qExp.fourth());
		if (qExp.second() == "")
			qExp.setSecond("0");
		value l_value = readValue(qExp.second());
		value r_value = readValue(qExp.third());
		string result = l_value - r_value;
		assignProcess(tempSymbol,result);
	}
	else if (type == quarterExp::MUL) {
		string result = "";
		symbol tempSymbol = m_symbolTable.getSymbol(qExp.fourth());
		if (qExp.second() != "") {
			value l_value = readValue(qExp.second());
			value r_value = readValue(qExp.third());
		}
		assignProcess(tempSymbol,  result);
	}
	else if (type == quarterExp::DIV) {
		symbol tempSymbol = m_symbolTable.getSymbol(qExp.fourth());
		value l_value = readValue(qExp.second());
		value r_value = readValue(qExp.third());
		string result = l_value / r_value;
		assignProcess(tempSymbol, result);
	}
	else if (type == quarterExp::GT) {
		symbol tempSymbol = m_symbolTable.getSymbol(qExp.fourth());
		value l_value = readValue(qExp.second());
		value r_value = readValue(qExp.third());
		string result = l_value > r_value;
		assignProcess(tempSymbol,  result);
	}
	else if (type == quarterExp::GET) {
		symbol tempSymbol = m_symbolTable.getSymbol(qExp.fourth());
		value l_value = readValue(qExp.second());
		value r_value = readValue(qExp.third());
		string result = l_value >= r_value;
		assignProcess(tempSymbol,result);
	}
	else if (type == quarterExp::LT) {
		symbol tempSymbol = m_symbolTable.getSymbol(qExp.fourth());
		value l_value = readValue(qExp.second());
		value r_value = readValue(qExp.third());
		string result = l_value < r_value;
		assignProcess(tempSymbol, result);
	}
	else if (type == quarterExp::LET) {
		symbol tempSymbol = m_symbolTable.getSymbol(qExp.fourth());
		value l_value = readValue(qExp.second());
		value r_value = readValue(qExp.third());
		string result = l_value <= r_value;
		assignProcess(tempSymbol, result);
	}
	else if (type == quarterExp::EQ) {
		symbol tempSymbol = m_symbolTable.getSymbol(qExp.fourth());
		value l_value = readValue(qExp.second());
		value r_value = readValue(qExp.third());
		string result = l_value == r_value;
		assignProcess(tempSymbol, result);
	}
	else if (type == quarterExp::NEQ) {
		symbol tempSymbol = m_symbolTable.getSymbol(qExp.fourth());
		value l_value = readValue(qExp.second());
		value r_value = readValue(qExp.third());
		string result = nequal(l_value, r_value);
		assignProcess(tempSymbol, result);
	}
	else if (type == quarterExp::CALL) {
		m_previousEbp.push(m_ebp);
		m_ebp = m_esp - 1;
	}
	else if (type == quarterExp::CALLFH) {
		m_valueVec.erase(m_valueVec.end() - 1);
		m_esp = m_ebp;
		m_ebp = m_previousEbp.top();
		m_previousEbp.pop();
	}

	
}

void execute::inputToStream(string value)
{
	m_inputStream.clear();
	m_inputStream.str("");
	m_inputStream << value;
}



bool execute::isOver()
{
	return m_isOver;
}

void execute::setIsOver(bool isOver)
{
	m_isOver = isOver;
}

void execute::run()
{
	m_isOver = false;
	while (!m_isOver)
		runSingleStmt();
}

//根据string返回类型（0:int, 1:float, 2:variable, 3:temp)
int execute::getStringType(string str)
{
	if ('0' < str[0] < '9') {
		for (int i = 1; i < str.size(); i++) {
			if (str[i] == '.')
				return 1;
		}
		return 0;
	}else if(str[0] == '#'){
		return 3;
	}
	else {
		return 2;
	}
}

/*
根据字符串读取值
纯数字：REAL\ INT
'开头为CHAR
$开头为地址
#开头为临时变量
@开头为栈顶栈底
"开头为STR
*/
value execute::readValue(string str)
{
	if (isdigit(str[0]) || str[0] == '-') {
		for (int i = 1; i < str.size(); i++) {
			if (str[i] == '.')
				return value(str, value::REAL_VALUE);
		}
		return value(str, value::INT_VALUE);
	}
	else if (str[0] == '\'') {
		return value(str, value::CHAR_VALUE);
	}
	else if (str[0] == '$') {
		return value(str, value::ADDR);
	}
	else if (str[0] == '#') {
		return m_tempMap[str];
	}
	else if (str[0] == '@') {
		int index = getRegNum(str);
		return m_valueVec[index];
	}
	else if (str[0] == '"') {
		int returnAddr = m_curConstAddr;
		for (int i = 1; i < str.size() - 1; i++) {
			m_constVec.push_back(value("'" + ctos(str[i]) + "'", value::CHAR_VALUE));
			m_constMap[m_constVec.size() - 1] = m_curConstAddr;
			m_curConstAddr = m_curConstAddr + 1;
		}
		m_constVec.push_back(value("''", value::CHAR_VALUE));
		m_constMap[m_constVec.size() - 1] = m_curConstAddr;
		m_curConstAddr = m_curConstAddr + 1;
		return value("$" + itos(returnAddr), value::ADDR);
	}
	else {
		symbol tempSymbol = m_symbolTable.getSymbol(str);
		switch (tempSymbol.type()) {
		case symbol::SINGLE_CHAR:
		case symbol::SINGLE_INT:
		case symbol::SINGLE_REAL:
			if (tempSymbol.level() == 0)
				return m_globalVec[tempSymbol.valueIndex()];
			else
				return m_valueVec[tempSymbol.valueIndex()];
		default:
			break;
		}
	}
}

void execute::assignProcess(symbol sym, string str)
{
	value tempValue = readValue(str);
	switch (sym.type()) {
	case symbol::TEMP:
		m_tempMap[sym.name()] = tempValue;
		break;
	case symbol::REG:
		if (sym.valueIndex() == m_esp) {
			m_valueVec.push_back(tempValue);
			m_addrMap[m_esp] = m_curDataAddr - tempValue.size();
			m_curDataAddr = m_addrMap[m_esp];
			m_esp++;
		}
		m_valueVec.at(sym.valueIndex()) = tempValue;
		break;
	case symbol::SINGLE_INT:
		assignUtil(sym.valueIndex(), 0, tempValue.valueStr(), value::INT_VALUE, sym.level() == 0);
		break;
	case symbol::SINGLE_REAL:
		assignUtil(sym.valueIndex(), 0, tempValue.valueStr(), value::REAL_VALUE, sym.level() == 0);
		break;
	case symbol::SINGLE_CHAR:
		assignUtil(sym.valueIndex(), 0, tempValue.valueStr(), value::CHAR_VALUE, sym.level() == 0);
		break;
	default:
		break;
	}
}

void execute::assignUtil(int valueIndex, int index, string valueStr, int valueType, bool isGlobal)
{
	if (isGlobal)
		m_globalVec[valueIndex + index] = value(valueStr, valueType);
	else
		m_valueVec[valueIndex - index] = value(valueStr, valueType);
}

void execute::declareProcess(quarterExp qExp, string type)
{
	int symbolType = -1;
	int funSymbolType = -1;
	int valueType = -1;
	string initValue = "0";
	int elementNum = 0;
	if (qExp.third() == "") {
		if (type == quarterExp::INT) {
			symbolType = symbol::SINGLE_INT;
			funSymbolType = funsymbol::INT;
			valueType = value::INT_VALUE;
		}
		else if (type == quarterExp::REAL) {
			symbolType = symbol::SINGLE_REAL;
			funSymbolType = funsymbol::REAL;
			valueType = value::REAL_VALUE;
		}
		else if (type == quarterExp::CHAR) {
			symbolType = symbol::SINGLE_CHAR;
			funSymbolType = funsymbol::CHAR;
			valueType = value::CHAR_VALUE;
			initValue = "''";
		}
		else if (type == quarterExp::VOID) {
			symbolType = funsymbol::VOID;
		}
		if (qExp.fourth()[qExp.fourth().size() - 1] == ')') {
			funsymbol funsym(qExp.fourth().substr(0, qExp.fourth().size() - 2), qExp.lineNo(), funSymbolType);
			m_symbolTable.registerFunSymbol(funsym);
		}
		else {
			if (qExp.second() != ""&&qExp.second()[0] == '@') {
				symbol sym(qExp.fourth(), symbolType, qExp.lineNo(), m_level, elementNum);
				sym.setValueIndex(getRegNum(qExp.second()));
				m_symbolTable.registerSymbol(sym);
				assignProcess(sym,  qExp.second());
			}
			else {
				symbol sym(qExp.fourth(), symbolType, qExp.lineNo(), m_level, elementNum);
				if (m_level == 0) {
					int globalIndex = m_globalVec.size();
					sym.setValueIndex(globalIndex);
					m_symbolTable.registerSymbol(sym);
					m_globalVec.push_back(value(initValue, valueType));
					m_globalMap[globalIndex] = m_curGlobalAddr;
					m_curGlobalAddr = m_globalMap[globalIndex] + sym.dataSize();
				}
				else {
					sym.setValueIndex(m_esp);
					m_symbolTable.registerSymbol(sym);
					m_valueVec.push_back(value(initValue, valueType));
					m_addrMap[m_esp] = m_curDataAddr - sym.dataSize();
					m_curDataAddr = m_addrMap[m_esp];
					m_esp++;
				}
				if (qExp.second() != "")
					assignProcess(sym,  qExp.second());
			}
		}
	}
}

int execute::getValueIndex(string addrValue, int &addrRange, int &offset)
{
	int addr = atoi(addrValue.substr(1, addrValue.size() - 1).c_str());
	if (addr <= DATA_ADDR && addr > 0x40000000) {//数据段
		addrRange = 1;
		for (int i = 0; i < m_valueVec.size(); i++) {
			if (addr >= m_addrMap[i]) {
				if (i + 1 < m_valueVec.size() && m_addrMap[i + 1] > m_addrMap[i]) {
					i += 1;
					
				}
				else {
					offset = addr - m_addrMap[i];
					return i;
				}
			}
		}
		throw executeException(m_executeStmtLineNo, "堆溢出");
	}
	else if (addr >= GLOBAL_ADDR && addr < 0x40000000) {//全局
		addrRange = 2;
		for (int i = 0; i < m_globalVec.size(); i++) {
			if (addr <= m_globalMap[i]) {
				if (addr < m_globalMap[i]) {
					offset = addr - m_globalMap[i - 1];
					return i - 1;
				}
				else {
					return i;
				}
			}
		}
		throw executeException(m_executeStmtLineNo, "堆溢出");
	}
	else if (addr >= CONST_ADDR && addr < GLOBAL_ADDR) {//常量
		addrRange = 3;
		for (int i = 0; i < m_constVec.size(); i++) {
			if (addr <= m_constMap[i]) {
				if (addr < m_constMap[i]) {
					offset = addr - m_constMap[i - 1];
					return i - 1;
				}
				else {
					return i;
				}
			}
		}
		throw executeException(m_executeStmtLineNo, "栈溢出");
	}
	else if (addr >= CODE_ADDR && addr < CONST_ADDR) {//代码区
		addrRange = 4;
		return (addr - CODE_ADDR) / 4;
	}
	else if (addr > DATA_ADDR) {
		throw executeException(m_executeStmtLineNo, "栈溢出");
	}
	else {
		return -1;
	}
}


int execute::getRegNum(string str)
{
	int result = 0;
	if (str.substr(1, 3) == "ebp") {
		if (str.size() > 4 && str[4] == '-')
			result = m_ebp - atoi(str.substr(5, str.size() - 5).c_str());
		else
			result = m_ebp;
	}
	else if (str.substr(1, 3) == "esp") {
		if (str.size() > 4 && str[4] == '-')
			result = m_esp - atoi(str.substr(5, str.size() - 5).c_str());
		else
			result = m_esp;
	}
	return result;
}


