#include <iostream>
#include <string>
#include <vector>

using std::endl;
using std::cout;
using std::cerr;

class Widget{};

void f1(Widget&& param); //no type deduction; rvalue referance

Widget&& var1 = Widget(); //no type deduction; rvalue referance

auto&& var2 = var1;//var2 is a universal reference

template<typename T>
void f2(std::vector<T>&& param) // rvalue reference
{
}

void ref_info(Widget& w)
{
	cout << "lreference" << endl;
}

void ref_info(Widget&& w)
{
	cout << "rreference" << endl;
}

template <typename T>
void f3(T&& param) // param is a universal reference
{
	ref_info(std::forward<T>(param));
}

template <typename... T>
void drop(T&&... args)
{}

int main(int argc, char * argv[])
{
	{
		cout << "# function template parameters" << endl;
		Widget w;
		f3(w);// lvalue passed to f; param's type is Widget&
		f3(std::move(w)); //  rvalue passed to f; param's type is Widget&&

#ifdef ERROR1
		std::vector<int> v;
		f2(v);//error! can't bind lvalue to rvalue
#endif
	}

	{
#if __cplusplus > 201103
		cout << "\n# auto" << endl;
		auto timeFuncInvocation = [](auto&& func, auto&&... params)
		{
			std::forward<decltype(func)>(func)(
						std::forward<decltype(params)>(params)...);
		};
		drop(timeFuncInvocation);
#endif
	}

	return 0;
}
