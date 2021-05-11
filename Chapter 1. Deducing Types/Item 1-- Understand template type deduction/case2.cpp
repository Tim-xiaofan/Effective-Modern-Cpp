/*20210427 16:18, zyj, GuangDong*/
//case2.cpp--ParamType is a Universal Reference

#include <iostream>

template<typename T>
void f(T&& param){}

int main()
{
	int x = 27; // as before
	const int cx = x; // as before
	const int& rx = x; // as before
	f(x); // x is lvalue, so T is int&,
	// param's type is also int&
	f(cx); // cx is lvalue, so T is const int&,
	// param's type is also const int&
	f(rx); // rx is lvalue, so T is const int&,
	// param's type is also const int&
	f(27); // 27 is rvalue, so T is int, (case1)
	// param's type is therefore int&&

	return 0;
}
