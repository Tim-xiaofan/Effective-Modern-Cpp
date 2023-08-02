#include <iostream>
#include <string>
#include <memory>
#include <type_traits>
#include <array>
#include <utility>
#include <iomanip>

using std::cout;
using std::endl;
using std::array;

/// C++11
constexpr int size = 10;
// constexpr函数的实参可以是编译期常量，也可以是运行期数值。如果实参是编译期常量，则求值过程在编译期进行，否则在运行期进行。
// 可在编译期求值
constexpr int fib(int n) {
    return n < 2 ? n : fib(n - 1) + fib(n - 2);
}
//C++11还支持将类的构造函数和成员函数声明为constexpr，这样就可以在编译期完成对象构造
class Circle {
public:
    constexpr Circle(double r) : radius(r) { }
    constexpr double Area() {
        return 3.14 * radius * radius;
    }
private:
    double radius;
};


/// C++14
// 在C++14中，constexpr关键字修饰函数不再要求所有逻辑必须在一个return语句中完成，可以使用局部变量和循环等
constexpr int fib1(int n) {
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        int c = a + b;
        a = b;
        b = c;
    }
    return a;
}

/// C++17
// 然而C++14标准在制定时并未将lambda表达式纳入constexpr表达范围之内，lambda表达式无法在编译期求值
// 替代方案
constexpr int foo() {
    class Functor {
    public:
        constexpr int operator()() {
            return 5;
        }
    };
    return Functor()();
}

constexpr auto foo(int a, int b) {
	return [a, b](int x) constexpr {
		return a * x + b;
	};
}

int main(void)
{
	{ // C++11 and C++14
		static_assert(fib(5) > fib(4));
		static_assert(fib1(5) > fib1(4));
		int n = 6;
		cout << fib(n) << endl; // 运行时
		// static_assert(fib(n) > 0);

		static_assert(Circle(2.0).Area() > 0);

		static_assert(foo() == 5, "");
	}

	{ // C++17
		static_assert(foo(1, 2)(5) > 0);
		// 若lambda表达式捕获的变量是字面量类型（literal type），则整个lambda表达式也将表现为字面量类型(隐式)
		auto add = [](int a, int b) -> int { return a + b; };
		static_assert(add(1, 2) == 3);
		int a = 1;
		int b = 2;
		cout << add(a, b) << endl; // 退化为普通lambda
		// static_assert(add(a, b)); // compile error
		// 增加constexpr lambda表达式语法 [capture_list] (params_list) constexpr -> return_type {function_body}
		auto sub = [](int a, int b) constexpr -> int { return a - b; };
		static_assert(sub(1, 1) == 0);

		// 在对constexpr lambda表达式对编译期求值过程中，遇到throw或new语句就会造成编译错误
		auto div = [](int a, int b) -> int {
			if(b == 0) throw "";
			else return a / b;
		};
		static_assert(div(1, 1) == 1); // 但是只要constexpr lambda表达式在编译期求值时没有经过throw或new语句，就不会产生编译器错
		// static_assert(div(1, 0)); // compile error
	}
	return 0;
}
