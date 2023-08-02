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

int main(void)
{
	/// 以前auto x {3}; 将被推导为std::initializer_list<int>, 现在推导为int
	// auto x1 {1, 2, 3}; // error: not a single element
	auto x2 = {1, 2, 3}; // x2 is std::initializer_list<int>
	auto x3 {3}; // x3 is int
	auto x4 {3.0}; // x4 is double

	// (void)x1;
	(void)x2;
	(void)x3;
	(void)x4;
	return 0;
}
