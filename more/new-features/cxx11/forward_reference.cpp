#include <iostream>
#include <vector>
#include <type_traits>
#include <utility>

#define NOT_USED(x) (void)x

class Widget
{
	public:
		//template<typename T>   // Version 1
    	//void f(const T& rhs) const  // Best match for const Lvalue Ref
		//{
		//	std::cout << __PRETTY_FUNCTION__ << std::endl;
		//}
		
		template<typename T>   // Version 2
    	void f(T&& rhs) const   // universal reference, can handle everything
		{
			std::cout << __PRETTY_FUNCTION__ << std::endl;
		}
};

namespace
{

Widget createWidget()
{
	return Widget();
}

template<typename T>
void f(std::vector<T>&& param); // Case 3: "&&" means rvalue reference

template<typename T>
void func(T&& param); // Case 4: "&&" does not mean rvalue reference, 
					  // it's forward reference because type deduce on T
} // namespace

namespace A
{

template<typename T>
void f(T&& param) // deduced parameter type T&&, so universal reference
{
	std::cout<< __PRETTY_FUNCTION__ << std::endl;
}

template<typename T>
void f1(const T&& param); // deduced parameter type const T&&, so rvalue reference

template<typename T>
void f2(std::vector<T>&& param); // deduced parameter type T, non-reference


template<typename T>
class MyVector
{
	void push_back(T&& rhs); // no type deduction here, already done, && rvalue reference

	template<typename... Args>
	void emplace_back(Args&&... args); //type deduction; && universal reference
};

void f3(Widget&& param); // no type deduction, rvalue reference

}

template<typename T1, typename T2>
void func(T1&& a1, T2&& a2)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

template<typename T1, typename T2>
void wrapper(T1&& e1, T2&& e2)
{
	func(std::forward<T1>(e1), std::forward<T2>(e2));
	// std::forward is a conditional cast but std::move is an unconditional cast
}

/// reference-collapsing context is typedef
namespace B
{

template<typename T>
class Widget {
	public:
		typedef T& LvalueRefType;
};

Widget<int&> w;
static_assert(std::is_same<typename Widget<int&>::LvalueRefType, int&>::value, "");
static_assert(std::is_same<typename Widget<int&&>::LvalueRefType, int&>::value, "");

void f(Widget<int&>::LvalueRefType&& param)
{
	static_assert(std::is_same<decltype(param), int&>::value, "");
}

}

class foo
{
	public:
		foo() { std::cout << __PRETTY_FUNCTION__ << std::endl; }
		foo(const foo& ) {  std::cout << __PRETTY_FUNCTION__ << std::endl; }
};

int main()
{
	{
		Widget&& var1 = createWidget(); // Case 1: "&&" means rvalue reference
		NOT_USED(var1);

		auto&& var2 = var1; // Case 2:  "&&" does not mean rvalue reference
		NOT_USED(var2);
	}

	// Univeral references in Templates
	{
		Widget w;
		A::f(w);
		// T&& forwarding reference will be resolved as Lvalue Ref
		// Thus instantiating void f(Widget&) out of the template
		// void f(T&&) [with T = Widget&].  // Printed

		const Widget cw;
		A::f(cw); 
		// T&& forwarding reference will be resolved as Lvalue Ref to const
		// Thus instantiating void f(const Widget&) out of the template
		//void f(T&&) [with T = const Widget&].  // Printed

		A::f(Widget());
		// T&& forwarding reference will be resolved as Rvalue Ref
		// Thus instantiating void f(Widget&&) out of the template
		// void f(T&&)[with T = Widget] // Why? Reference Collapsing 

		//const Widget cw;
		A::f(std::move(cw));
		// T&& forwarding reference will be resolved as Rvalue Ref to const
		// Thus instantiating void f(const Widget&&) out of the template
		// void f(T&&)[with T = const Widget]
		// Printed, Why? Explained later in Table 2(Reference Collapsing)
	}

	// Univeral references in auto
	{
		auto&& var = 10; // universal reference
		static_assert(std::is_same<int&&, decltype(var)>::value, "");

		std::vector<int> vec = {0, 1, 2};
		auto&& var1 = vec[0]; // universal reference
		static_assert(std::is_same<int&, decltype(var1)>::value, "");

		int i = 10;
		// const auto&& var2 = i; // Not a Universal Reference hence Error!!
		// NOT_USED(var2);
		NOT_USED(i);

		// auto&& var3 = NULL; //Werror=conversion-null 
		// static_assert(std::is_same<int&, decltype(var3)>::value, "");
	}

	// Take care of ...
	{
		Widget m;
		const Widget cm;
		m.f(m);            // Case 1, Calls Version 2!!
		m.f(std::move(m)); // Case 2, Calls Version 2
		m.f(cm);           // Case 3, Calls Version 1
		m.f(std::move(m)); // Case 4, Calls Version 2
	}

	// The perfect forwarding problem
	{
		wrapper(1, 2);
		int a;
		const int b = 0 ;
		wrapper(a, b);
	}

	/// Universal references in “typedef”
	{
	}

	/// reference-collapsing takes place is the use of decltype.
	{
		Widget w1, w2;
		NOT_USED(w2);

		auto&& v1 = w1;                  // v1 is an auto-based universal reference being
		                                 // initialized with an lvalue, so v1 becomes an
		                                 // lvalue reference referring to w1.
		NOT_USED(v1);
		
		// decltype’s type deduction depends only on the decltype expression; the type of the initializing expression (if any) is ignored
		//decltype(w1)&& v2 = w2;          // v2 is a decltype-based universal reference, and
		//                                 // decltype(w1) is Widget, so v2 becomes an rvalue reference.
		//                                 // w2 is an lvalue, and it’s not legal to initialize an
		//                                 // rvalue reference with an lvalue, so
		//                                 // this code does not compile.
		//NOT_USED(v2);

		// 当你需要某个表达式的返回值类型而又不想实际执行它时用decltype
		decltype(foo()) x;
		auto y = x; NOT_USED(y); // auto varname 必须要初始化,故foo()要执行
	}
}
