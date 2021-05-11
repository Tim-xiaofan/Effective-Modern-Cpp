/*20210427 16:18, zyj, GuangDong*/
//case3.cpp--ParamType is Neither a Pointer nor a Reference

#include <iostream>

template<typename T>
void f(T param){}// param is now passed by value

int main()
{
	int x = 27; // as before
	const int cx = x; // as before
	const int& rx = x; // as before
	f(x); // T's and param's types are both int
	f(cx); // T's and param's types are again both int
	f(rx); // T's and param's types are still both int

	const char* const ptr = // ptr is const pointer to const object
		"Fun with pointers";
	f(ptr);

	return 0;
}
