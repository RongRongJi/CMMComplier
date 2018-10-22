#pragma once
#include<vector>
#include<string>
using namespace std;

extern vector<int> errorVec;

//错误分析的相关输出
//parameter: 相关错误号:
//1.标识符以下划线结尾
//2.非合法的字符
//return: 报错信息
static string errorStr(int errorNo) {
	switch (errorNo) {
	case 1:
		return "词法错误：标识符以下划线结尾";
	case 2:
		return "词法错误：非合法的字符: ";
	}
}