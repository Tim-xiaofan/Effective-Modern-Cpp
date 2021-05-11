/*20210427 16:32, zyj, GuangDong*/
//array_param.cpp--

#include <iostream>
#include <array>

void someFunc(int, double){} // someFunc is a function type is void(int, double)
template<typename T>
void f1(T param){} // in f1, param passed by value
template<typename T>
void f2(T& param){} // in f2, param passed by ref

int main()
{
	f1(someFunc); // param deduced as ptr-to-func type is void (*)(int, double)
	f2(someFunc); // param deduced as ref-to-func type is void (&)(int, double)
	return 0;
}
