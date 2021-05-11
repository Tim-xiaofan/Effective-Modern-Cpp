/** 202105011 12:29, zyj, GuangDong*/
//template_ret.c
#include <iostream>
#include <deque>

using std::cout;
using std::endl;

class Widget
{
};

int main()
{
	Widget w;
	const Widget& cw = w;
	auto myWidget1 = cw; // auto type deduction: myWidget1's type is Widget
	cout << myWidget1 << endl;

	decltype(auto) myWidget2 = cw; // decltype type deduction:
	// myWidget2's type is
	// const Widget&
	cout << myWidget2 << endl;
	return 0;
}
