#include <iostream>
#include <string>
#include <memory>
#include <string.h>
#include <chrono>

using std::endl;
using std::cout;
using std::cerr;

namespace X
{
	template<typename _Tp>
    struct remove_reference
    { typedef _Tp   type; };

	template<typename _Tp>
    struct remove_reference<_Tp&>
    { typedef _Tp   type; };

	template<typename _Tp>
    struct remove_reference<_Tp&&>
    { typedef _Tp   type; };

	template<typename T>
	typename remove_reference<T>::type&&
	move(T&& param)
	{
		using ReturnType =
			typename remove_reference<T>::type&&;
		return static_cast<ReturnType>(param);
	}
}

namespace Y
{
	/**
	 *  @brief  Forward an lvalue.
	 *  @return The parameter cast to the specified type.
	 *
	 *  This function is used to implement "perfect forwarding".
	 */
	template<typename _Tp>
	constexpr _Tp&&
	forward(typename std::remove_reference<_Tp>::type& __t) noexcept
	{ return static_cast<_Tp&&>(__t); }

	/**
	 *  @brief  Forward an rvalue.
	 *  @return The parameter cast to the specified type.
	 *
	 *  This function is used to implement "perfect forwarding".
	 */
	template<typename _Tp>
	constexpr _Tp&&
	forward(typename std::remove_reference<_Tp>::type&& __t) noexcept
	{
		static_assert(!std::is_lvalue_reference<_Tp>::value, "template argument"
					" substituting _Tp is an lvalue reference type");
		return static_cast<_Tp&&>(__t);
	}

	class Widget
	{
		public:
			Widget() = default;
			Widget(const Widget& rhs)
			{
				cout << "Widget::copy constuctor" << endl;
			}
			Widget(Widget&& rhs)
			{
				cout << "Widget::move constuctor" << endl;
			}
		private:
			static std::size_t moveCtorCalls;
	};

	std::size_t Widget::moveCtorCalls = 0;

	void process(const Widget& lvalArg) // process lvalues
	{
		cout << "process lvalues" << endl;
	}
	void process(Widget&& rvalArg) // process rvalues
	{
		cout << "process rvalues" << endl;
	}

	template<typename T> // template that passes
	void logAndProcess(T&& param) // param to process
	{
		cout << "lvalue reference:" << std::boolalpha << std::is_lvalue_reference<T>::value << endl;
	 	process(forward<T>(param));
	}
	
	template<typename T> // template that passes
	void logAndProcess1(T&& param) // param to process
	{
	 	process(param);
	}

	class Annotation
	{
		public:
			explicit Annotation(const Widget text):
				value(std::move(text)){}
		private:
			Widget value;
	};
}

template <typename T>
void foo(T& t)
{
	cout << "l reference" << endl;
}

template <typename T>
void foo(T&& t)
{
	cout << "r reference" << endl;
}


int main(int argc, char * argv[])
{
	{
		cout << "# implemention of std::move" << endl;
		int i = 0;
		int & ri = i;
		foo(i);
		foo(ri);
		foo(std::move(i));
		foo(X::move(i));
		foo(static_cast<int &&>(ri));
	}

	{
		cout << "\n# lvalue-reference-to-const is permitted to "
			"bind to a const rvalue" << endl;
		Y::Widget w;
		Y::Annotation a(w);
	}

	{
		cout << "\n# std::forward" << endl;
		Y::Widget w;
		cout << "with std::forward" << endl;
		Y::logAndProcess(w);
		Y::logAndProcess(std::move(w));
		cout << "without std::forward" << endl;
		Y::logAndProcess1(w);
		Y::logAndProcess1(std::move(w));
#ifdef ERROR
		Y::forward<int&>(5);
		Y::forward<std::string&>(std::string{});
#endif
	}
	return 0;
}
