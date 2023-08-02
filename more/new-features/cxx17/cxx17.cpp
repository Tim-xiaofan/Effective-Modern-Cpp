#include <iostream>
#include <string>
#include <memory>
#include <type_traits>
#include <array>
#include <utility>

using std::cout;
using std::endl;
using std::array;

int main(void)
{
	{ /// 类模板的模板参数推导
		// 模板类构造函数能自动推导模板参数的类型(Class template argument deduction, CTAD)。例如允许以pair(5.0, false) 取代pair<double,bool>(5.0, false)
		auto i = std::pair(1, 2);
		auto f = std::pair(1.0, 2.0);
		(void)i; (void)f;
	}

	{ ///
	}
	return 0;
}
