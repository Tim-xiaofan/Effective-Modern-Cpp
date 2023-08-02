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

using std::cout;
using std::endl;
using std::array;

/// C++17新增了std::variant来实现类似union的功能。那么既然有了union，为何还需要variant呢？
/// Every time you want to use a union use std::variant

/// 这是因为union中只能有基础类型，像std::string、std::map等复杂类型是不能出现在union中的，而std::variant却可以

int main() 
{
	// 基本用法
	{
		std::variant<int, std::string> v1(312);
		std::variant<int, std::string> v2("Hello, Variant");
		cout << "v1.index(): " << v1.index() << endl;
		cout << "v2.index(): " << v2.index() << endl;

		//通过类型取值
		const auto& i = std::get<int>(v1);
		cout << "i: " << i << endl;
		const auto& str = std::get<std::string>(v2);
		cout << "str: " << str << endl;

		// 通过索引取值
		const auto& i1 = std::get<0>(v1);
		cout << "i1: " << i1 << endl;
		const auto& str1 = std::get<1>(v2);
		cout << "str1: " << str1 << endl;

		try {
			std::get<std::string>(v1);
		} catch(const std::bad_variant_access& e) {
			cout << e.what() << endl;
		}

		// 重置
		v2 = 512;
		cout << "v2.index(): " << v2.index() << endl;
	}

	// 更安全的获取方法
	{
		std::variant<int, std::string> v1(1);
		std::variant<int, std::string> v2("str");
		if(std::get_if<int>(&v1) != nullptr)
		{
			std::cout << "correct type" << endl;
		}
		else
		{
			std::cout << "wrong type" << endl;
		}
	}

    return 0;
}
