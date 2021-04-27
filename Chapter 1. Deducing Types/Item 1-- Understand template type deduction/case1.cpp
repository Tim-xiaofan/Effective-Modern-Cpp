/*20210427 16:09, zyj, GuangDong*/
//case1.cpp--ParamType is a Reference or Pointer, but not a Universal Reference

#include <iostream>

template<typename T>
void f(T& param){}

int main()
{
	int x = 27; // x is an int
	const int cx = x; // cx is a const int
	const int& rx = x; // rx is a reference to x as a const int
	f(x); // T is int, param's type is int&
	f(cx); // T is const int,
	// param's type is const int&
	f(rx); // T is const int,
	// param's type is const int&

	return 0;
}
