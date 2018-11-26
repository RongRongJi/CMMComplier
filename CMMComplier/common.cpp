#include "common.h"


std::string itos(int n)
{
	std::stringstream ss;
	ss << n;
	return ss.str();
}


std::string dtos(double d)
{
	std::stringstream ss;
	ss << d;
	return ss.str();
}


std::string ctos(char ch)
{
	std::stringstream ss;
	ss << ch;
	return ss.str();
}
