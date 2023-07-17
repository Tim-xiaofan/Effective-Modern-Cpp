#include <iostream>
#include <string>
#include <memory>
#include <type_traits>
#include <array>

using std::cout;
using std::endl;
using std::array;

namespace A
{
//函数返回类型推导
auto correct(int i)
{
	if(i == 1)
	{
		return 1;
	}
	else
	{
		return correct(i-1) + i;
	}
}
//auto wrong(int i)
//{
//	if(i != 1)
//	{
//		return wrong(i-1) + i;
//	}
//	else
//	{
//		return i;
//	}
//}
}//namespace A

int&& f() 
{
	static int value = 10;
    return std::move(value);
}

namespace B
{
// Return type is `int`.
auto f(const int& i) {
    return i;
}
// Return type is `const int&`.
decltype(auto) g(const int& i) {
    return i;
}
int CPP()
{
    // Note: Especially useful for generic code!
    int x = 123;
    static_assert(std::is_same<const int&, decltype(f(x))>::value == false);
    static_assert(std::is_same<int, decltype(f(x))>::value == true);
    static_assert(std::is_same<const int&, decltype(g(x))>::value == true);
    return 0;
}
}

namespace C
{

int j = 0;
constexpr int i = 42;

// constexpr函数
constexpr int new_sz() { return 42; }//constexpr函数
constexpr int foo = new_sz();
//在对变量foo初始化时，编译器把对constexpr函数的调用替换成其结果值。为了能在编译过程中随时展开，constexpr函数被隐式地指定为内联函数。
constexpr int new_sz1(int x) { return x; }
constexpr int foo1 = new_sz1(foo);
//constexpr int foo2 = new_sz1(j); 

// C++11
constexpr int factorial1(int n)
{
    return n == 0 ? 1 : n * factorial1(n - 1);
}
// C++14
// C++14放松了这些限制
constexpr int factorial2(int n)
{
    int result = 1;
    for (int i = 1; i <= n; ++i)
        result *= i;
    return result;
}

}

namespace D
{

// C++11
template<typename T>
class foo {};

template<typename T> using foos = foo<T>; 

template<typename T>
T func(T t) { return t; }

template<typename T> using funcs = foo<T>; 

// C++14
template<typename T>
constexpr T pi = T(3.141592653589793238462643383);

// 适用于特化规则 :
template<>
constexpr const char* pi<const char*> = "pi";
}

int main(void)
{
	{//泛型的lambda
		//在C++11中，lambda函数的形式参数需要被声明为具体的类型。C++14放宽了这一要求，允许lambda函数的形式参数声明中使用类型说明符auto。
		auto add = [](auto x, auto y) { return x + y; };
		cout << "1 + 1 = " << add(1, 1) << endl;
		cout << "2.7 + 1.3 = " << add(2.7, 1.3) << endl;
		cout << "\"Hello \" + \"C++14\" = " << add(std::string("Hello "), "C++14") << endl << endl;
	}

	{//Lambda捕获部分中使用表达式
		//使用标准函数std::move可以使之被用以通过move捕获
		auto ptr = std::make_unique<int>(10); 
		auto f = [value=std::move(ptr)]() { return *value; };
		std::cout << f() << std::endl << std::endl;
	}

	{//C++14允许lambda成员用任意的被捕获表达式初始化。这既允许了capture by value-move，也允许了任意声明lambda的成员，而不需要外层作用域有一个具有相应名字的变量。这称为广义捕获
		int x = 1;
		auto f = [&r = x, x = x * 10]
		{
			++r;
			return x + r;
		};
		auto f1 = [value = 1]{ return value; };
		std::cout << f1() << std::endl;
		std::cout << f() << std::endl << std::endl; 
	}

	{//新增decltype(auto)
		//C++11
		int i;
		int&& f();
		auto x3a = i;              // x3a的类型是int
		decltype(i) x3d = i;       // x3d的类型是int
		auto x4a = (i);            // x4a的类型是int
		decltype((i)) x4d = (i);   // x4d的类型是int&
		auto x5a = f();            // x5a的类型是int
		decltype(f()) x5d = f();   // x5d的类型是int&&
		(void)x3a; (void)x3d; (void)x4a;
		(void)x4d; (void)x5a; (void)x5d;
		//C++14增加了decltype(auto)的语法。允许auto的类型声明使用decltype的规则
		const int x = 0;
		auto x1 = x; // int
		static_assert(std::is_same<int, decltype(x1)>::value);
		decltype(auto) x2 = x; //const int
		static_assert(std::is_same<const int, decltype(x2)>::value);
		int y = 0;
		int& y1 = y;
		auto y2 = y1; // int
		decltype(auto) y3 = y1; // int&
		static_assert(std::is_same<int&, decltype(y3)>::value);
		int&& z = 0;
		auto z1 = std::move(z); // int
		static_assert(std::is_same<int, decltype(z1)>::value);
		decltype(auto) z2 = std::move(z); // int&&
		static_assert(std::is_same<int&&, decltype(z2)>::value);
		z = 9;
		std::cout << z2 << std::endl << std::endl;
		(void)x1; (void)x2;
		(void)y1; (void)y2; (void)y3;
		(void)z1; (void)z2;
	}

	{//constexpr
		{// 1.常量表达式: 指值不会改变并且在编译过程中就能够得到计算结果的值
			const int max_files = 20;            //max_files是常量表达式
			array<int, max_files> a; (void)a;
			const int limit = max_files + 1;     //limit是常量表达式
			array<int, limit> b; (void)b;
			int staff_size = 27; (void) staff_size;                 //staff_size不是常量表达式，因为它的数据类型不是const
			//array<int, staff_size> c; (void)c; //compile error
			std::string str("abc");
			const int sz = str.size(); (void) sz;
			//array<int, sz> d; (void)d; //compile error
			//sz不是常量表达式，因为get_size()只有到运行的时候才知道结果，除非它是constexpr修饰的(C++ 11标准）
		}
		{// 2.constexpr变量
			constexpr int mf = 20; (void)mf;              //正确，20是常量表达式
			constexpr int limit = mf + 1; (void)limit;       //正确，mf + 1是常量表达式
			std::string str("abc"); (void) str;
			//constexpr int sz = str.size();         //未知，若size()函数是一个constexpr函数时即正确，反之错误。

			int i = 10; (void)i;
			//constexpr int t = i;                //错误，i不是常量
		}
		{// 5. 指针与constexpr 
			const int *p = nullptr; (void)p;           //正确，p是一个指向整型常量的指针
			constexpr int *q = nullptr; (void)q;       //正确，但q是一个指向  整数  的  常量指针

			//i和j都必须定义在函数体之外，否则constexpr指针无法指向。
			constexpr const int *p1 = &C::i; (void)p1;                 /*p是常量指针，指向  整型常量i*/
			constexpr int *p2 = &C::j; (void)p2;                     //p1是常量指针，指向  整数j(非常量)
		}
		{// https://zh.wikipedia.org/zh-cn/C%2B%2B14#%E6%94%BE%E6%9D%BE%E7%9A%84constexpr%E5%87%BD%E6%95%B0%E9%99%90%E5%88%B6
			array<int, C::factorial1(3)> a1; (void)a1;
			array<int, C::factorial2(3)> a2; (void)a2;
		}
	}

	{// 变量模板
		//在C++之前的版本中，模板可以是函数模板或类模板（C++11引入了类型别名模板）。C++14现在也可以创建变量模板
		int i = D::pi<int>; (void)i;
		float f = D::pi<float>; (void)f;
		const char* str = D::pi<const char*>; (void)str;
	}
	
	return 0;
}
