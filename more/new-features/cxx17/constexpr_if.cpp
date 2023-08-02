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

namespace A
{

/// 传统的if-else语句是在运行期进行判断和选择的，无法运用在编译期，所以在泛型编程中，无法使用if-else来直接做一些判断。
/// 在C++17之前，只能被拆分为一个泛型版本和一个特化版本
void print()
{
	cout << endl;
}

template <typename T, typename... Args>
void print(T head, Args... tail)
{
	cout << head << ",";
	print(tail...);
}

} // namespace A

namespace B
{
/// C++17引入了constexpr if，支持在编译期进行判断，即在编译期就可以直接知道结果，可以广泛应用于泛型编程
template <typename T, typename... Args>
void print(T head, Args... tail)
{
	if constexpr(sizeof...(tail) > 0)
	{
		cout << head << ",";
		print(tail...);
	}
	else
	{
		cout << head << endl;
	}
}
} // namespace B


namespace C
{
template<typename... Args>
void print(Args... args)
{
	((cout << args << ",") , ...);
}
}


/// 再有，constexpr if在泛型编程中的一个应用是，可以替代冗长的std::enable_if的写法
namespace D
{

template<typename T>
std::enable_if_t<std::is_integral<T>::value, std::string> to_string(const T& t)
{
	return std::to_string(t);
}

template<typename T>
std::enable_if_t<!std::is_integral<T>::value, std::string> to_string(const T& t)
{
	return t;
}

} //namespace D

namespace E
{

template<typename T>
std::string to_string(const T& t)
{
	if constexpr (std::is_integral<T>::value)
	{
		return std::to_string(t);
	}
	else
	{
		return t;
	}
}

}

int main(void)
{
	{
		A::print();
		A::print(1, 2, 3);
		cout << endl << endl;
	}

	{
		//B::print();
		B::print(1, 2, 3, 4);
		cout << endl << endl;
	}

	{
		C::print();
		B::print(1, 2, 3, 4);
		cout << endl << endl;
	}

	/// 替代enable_if
	{
		cout << "to_string(12): " << D::to_string(12) << endl;
		// cout << "to_string(12.5): " << D::to_string(12.5) << endl; // comile error
	}
	{
		cout << "to_string(12): " << E::to_string(12) << endl;
		// cout << "to_string(12.5): " << E::to_string(12.5) << endl; // comile error
	}
	return 0;
}
