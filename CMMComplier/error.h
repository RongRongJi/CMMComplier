#pragma once
#include<vector>
#include<string>
using namespace std;

extern vector<int> errorVec;

//���������������
//parameter: ��ش����:
//1.��ʶ�����»��߽�β
//2.�ǺϷ����ַ�
//return: ������Ϣ
static string errorStr(int errorNo) {
	switch (errorNo) {
	case 1:
		return "�ʷ����󣺱�ʶ�����»��߽�β";
	case 2:
		return "�ʷ����󣺷ǺϷ����ַ�: ";
	}
}