#include <iostream>

using std::endl;
using std::cout;
using std::cerr;

class Widget{};

void info(Widget&, std::false_type)
{
	cout << "Widget&" << endl;
}

void info(Widget, std::true_type)
{
	cout << "Widget" << endl;
}

template<typename T>
void func(T&& param) 
{// 不考虑const, volatile时 
	 info(param, std::is_same<T, 
		 typename std::remove_reference<T>::type>());
}

Widget widgetFactory()
{// function returning rvalue
	return Widget();
}

namespace X
{
	template<typename T>
	T&& forward(typename std::remove_reference<T>::type& t)
	{
		return static_cast<T&&>(t);
	}
}

int main(int argc, char * argv[])
{
	{
		cout << "# lvalues are encoded as lvalue references, "
			"but rvalues are encoded as non-references" << endl;
		Widget w;

		func(w);// call func with lvalue; T deduced to be Widget&

		func(widgetFactory()); // call func with rvalue; T deduced to be Widget
	}

#ifdef ERROR1
	{
		cout << "error! can't declare reference to reference" << endl;
		auto& & rx = x;
	}
#endif
	return 0;
}
