/*20210427 17:41, zyj, GuangDong*/
//auto.cpp
//Case 1: The type specifier is a pointer or reference, but not a universal reference
//Case 2: The type specifier is a universal reference
//Case 3: The type specifier is neither a pointer nor a reference

#include <iostream>

void someFunc(int, double){} // someFunc is a function type is void(int, double)

int main()
{
	auto x = 27; // case 3 (x is neither ptr nor reference)
	const auto cx = x; // case 3 (cx isn't either)
	const auto & rx = x; // case 1 (rx is a non-universal ref.)

	//Case 2 works as you’d expect:
	auto&& uref1 = x; // x is int and lvalue, so uref1's type is int&
	auto&& uref2 = cx; // cx is const int and lvalue, so uref2's type is const int&
	auto&& uref3 = 27; // 27 is int and rvalue, so uref3's type is int&&

	const char name[] = // name's type is const char[13]
		"R. N. Briggs";
	auto arr1 = name; // arr1's type is const char*
	auto& arr2 = name; // arr2's type is const char (&)[13]

	auto func1 = someFunc; // func1's type is void (*)(int, double)
	auto& func2 = someFunc; // func2's type is void (&)(int, double)


	int x1 = 27;
	int x2(27);
	int x3 = { 27 };
	int x4{ 27 };
	auto x11 = 27; // type is int, value is 27
	std::cout << x11 << std::endl;
	auto x22(27); // ditto
	std::cout << x22 << std::endl;
	auto x33 = { 27 }; // type is std::initializer_list<int>,  value is { 27 }
	//std::cout << x33 << std::endl;
	auto x44{ 27 }; // ditto
	//std::cout << x44 << std::endl;

	return 0;
}
