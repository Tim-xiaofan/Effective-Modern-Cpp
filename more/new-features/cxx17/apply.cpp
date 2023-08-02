#include <iostream>
#include <string>
#include <memory>
#include <type_traits>
#include <array>
#include <utility>
#include <iomanip>
#include <cassert>
#include <thread>
#include <unistd.h>
#include <tuple>
#include <cassert>

using std::cout;
using std::endl;
using std::array;

//C++17引入std::apply，定义在标准库头文件<tuple>中，可以将类元组std::tuple、std::pair等作为函数参数进行传递，其原型声明为：
//
//template<class F, class Tuple>
//constexpr decltype(auto) apply(F&& f, Tuple&& t);
//其中f是可调用对象，t是一个元组类型，其元素将作为f的参数进行传递，返回值是f的返回结果

static int add(int a, int b)
{
	return a + b;
}

template<typename T>
T add_generic(T first, T second) { return first + second; }

auto add_lambda = [](auto first, auto second) { return first + second; };

template<typename... Ts>
std::ostream& operator<<(std::ostream& os, const std::tuple<Ts...>& theTuple)
{
	auto f = [&os](const Ts&... args) -> std::ostream& 
	{ 
		int n = 0;
		return ((os << args << (++n != sizeof...(args)?",":"")) , ...); 
	};
	return std::apply(f, theTuple);
}

namespace A
{

}

int main(void) 
{
	assert(std::apply(add, std::pair(1, 2)) == 3);
	assert(std::apply(add, std::tuple(1, 2)) == 3);

	// Error: can't deduce the function type
	// std::cout << std::apply(add_generic, std::make_pair(2.0f, 3.0f)) << '\n';

	// OK
    std::cout << std::apply(add_lambda, std::make_pair(2.0f, 3.0f)) << '\n';

	// advanced example
    std::tuple myTuple{25, "Hello", 9.31f, 'c'};
    std::cout << myTuple << '\n';

    return 0;
}
