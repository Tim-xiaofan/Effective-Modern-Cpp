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
#include <variant>
#include <optional>
#include <cassert>

using std::cout;
using std::endl;
using std::array;

/// C++17新增了std::optional可以用来返回空值，避免了使用智能指针并管理内存的麻烦
std::optional<int> StoI(const std::string& str) {
    try {
        return std::stoi(str);
    } catch(...) {
        return std::nullopt;
    }
}

int main() 
{
	{
		std::string str{"123str"};
		std::optional<int> i = StoI(str);
		if (i != std::nullopt)
		{
			// 使用*操作符直接获取值, 访问没有value的optional的时候, 行为是未定义的
			std::cout << *i << std::endl; 
		}
		else
		{
			std::cout << "invalid" << std::endl;
		}
	}

	{// has_value()
		std::optional<unsigned> opt;
		assert(!opt.has_value());
		opt = 12;
		assert(opt.has_value());
	}

	{ // value(), value_or()
		std::optional<unsigned> opt;
		try {
			// 当没有 value 时调用该方法将 throws std::bad_optional_access 异常
			opt.value();
		} catch(const std::bad_optional_access& e) {
			std::cerr << e.what() << std::endl;
		}
		std::cout << opt.value_or(9999) << std::endl;
	}
    return 0;
}
