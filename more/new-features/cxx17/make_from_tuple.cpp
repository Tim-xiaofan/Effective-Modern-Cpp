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

/// C++17引入std::make_from_tuple，可以将std::tuple展开作为构造函数的参数
struct Foo {
	Foo(int first, float second, int third) {
        std::cout << first << ", " << second << ", " << third << std::endl;
    }
};

int main(void) 
{
	auto tuple = std::make_tuple(25, 3.14f, 10);
	std::make_from_tuple<Foo>(std::move(tuple));
    return 0;
}
