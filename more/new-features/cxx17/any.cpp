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
#include <any>
#include <cassert>

using std::cout;
using std::endl;
using std::array;


int main()
{
	std::any foo = 1;
    if (foo.has_value())
	  std::cout << foo.type().name() << ": " << std::any_cast<int>(foo) << std::endl; //i: 1

    foo = 3.14f;
    if (foo.has_value())
	{
        std::cout << foo.type().name() << ": " << std::any_cast<float>(foo) << std::endl; //f: 3.14
	}

	try {
		std::cout << foo.type().name() << ": " << std::any_cast<int>(foo) << std::endl; //f: 3.14
	} catch(const std::bad_any_cast& e) {
		std::cerr << e.what() << std::endl;
	}
    
	foo.reset(); //重置数据，取消存储
    if (foo.has_value())
	{
        std::cout << foo.type().name() << std::endl;
	}
	return 0;
}
