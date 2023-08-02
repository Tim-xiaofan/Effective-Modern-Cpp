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

using std::cout;
using std::endl;
using std::array;

/// 捕获this指针在多线程环境中面临的问题
struct Foo {
    int val;
    void bar() {
		val = 9;
        auto f = [*this]() { std::cout << val << std::endl; }; //注意这里*this，不是this
        f();
    }
};

int main() {
    Foo foo;
    foo.bar();
    return 0;
}
