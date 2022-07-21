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

	/**
	  Widget& && forward(Widget& t)
	  {
		return static<Widget& &&>(t);
	  }
	  reference collapsing -->
	  Widget& forward(Widget& t)
	  {
		return static_cast<Widget&>(t);
	  }
	 */

	/**
	Widget&& && forward(Widget& t)
	{
		return static_cast<Widget&& &&>(t);
	}
	reference collapsing -->
	Widget&& forward(Widget& t)
	{
		return static_cast<Widget&&>(t);
	}
	*/

	template<typename T>
	class Widget
	{
		public:
			typedef T&& RvalueRefToT;
	};
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

	{
		cout << "\n# Reference collapsing occurs in four contexts: auto variables" << endl;
		Widget w;
		auto&& w1 = w; //lvalue: Widget& && w1 --> Widget&
		auto&& w2 = widgetFactory(); // rvalue: Widget && w1 -->Widget&&
		cout << "int& = int: " << std::boolalpha << std::is_same<int&, int>::value << endl;
		cout << "w1 = Widget&: " << std::boolalpha << std::is_same<decltype(w1), Widget&>::value << endl;
		cout << "w2 = Widget&&: " << std::boolalpha << std::is_same<decltype(w2), Widget&&>::value << endl;
	}

	{	
		cout << "\n# Reference collapsing occurs in four contexts: typedefs and alias" << endl;
		cout << "int& && = int&: " << std::boolalpha 
			<< std::is_same<typename X::Widget<int&>::RvalueRefToT, 
			int&>::value << endl;
	}

	{
		cout << "\n# Reference collapsing occurs in four contexts: decltype" << endl;
	}
	return 0;
}
