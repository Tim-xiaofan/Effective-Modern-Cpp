//UDLs.cpp, User-Defined Literals allows you to define your custom literal suffixes for various types.
#include <iostream>
#include <chrono>
#include <array>
#include <cstring>
#include <stdexcept>
#include <cassert>
#include <string>

namespace
{

// These custom suffixes can be appended to literal values
// to create objects of user-defined types or to apply custom conversions to standard literals
constexpr auto operator""_h(unsigned long long h)
{
	return std::chrono::hours(h);
}

constexpr auto operator""_h(long double h)
{
	return std::chrono::duration<long double, std::ratio<3600, 1>>(h);
}

template<typename Rep, typename Period>
std::ostream& operator<<(std::ostream& os, const std::chrono::duration<Rep, Period>& dur)
{
	using T = std::chrono::duration<Rep, Period>;
	return os << "{count: " << dur.count()
		<< ", numerator=" << T::period::num
		<< ", denominator=" << T::period::den
		<< "}";
}

constexpr int len(const char* str)
{
    return *str ? 1 + len(str + 1) : 0;
}

constexpr auto operator""_b(const char* binaryString)
{
	unsigned long long result = 0;
	int length = len(binaryString);
	for(int i = 0; i < length; ++i)
	{
		char bit = binaryString[i];
		if(bit == '1')
		{
			result += (1 << (length - i - 1));
		}
		else if(bit != '0')
		{
			throw std::invalid_argument("");
		}
	}
	return result;
}

}

int main(void)
{
	{ // 自定义
		constexpr auto hours = 5_h;
		std::cout << "hours: " << hours << std::endl;
		constexpr auto halfHour = 0.5_h;
		std::cout << "halfHour: " << halfHour << std::endl;
	}

	{ // 标准库
		using namespace std::chrono_literals;
		constexpr auto hours = 5.0h;
		static_assert(hours.count());
		constexpr auto hours1 = 4h;

		constexpr auto minutes = 45min;
		constexpr auto seconds = 9.85s;
		constexpr auto milliseconds = 150ms;
		constexpr auto microseconds = 0.5us;
		constexpr auto nanoseconds = 7ns;
	}

	{ // 自定义二进制字面量
		constexpr auto i = 110_b;
		static_assert(i == 6);
	}
	return 0;
}
