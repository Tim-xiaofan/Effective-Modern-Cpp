#include <iostream>
#include <chrono>

// 时钟节拍(时间精度)：
template <intmax_t N, intmax_t D = 1> class ratio;
// 其中N表示分子(numerator)，D表示分母(denominator)，默认用秒表示的时间单位。
// N对应于其成员num，D对应于其成员den
// 常用的单位：
// ratio<60, 1>                 minute
// ratio<1, 1>                  second
// ratio<1, 1000>               millisecond
// It is used to represent compile-time rational constants(constexpr)

using MicrosecondTickPeriod = std::ratio<1, 1000000>;
using MillisecondTickperiod = std::ratio<1, 1000>;
using SecondTickperiod = std::ratio<1, 1>;
using MinuteTickperiod = std::ratio<60, 1>;
using HourTickperiod = std::ratio<60 * 60, 1>;

int main()
{
    // Define a duration with a tick period of microseconds
    using Microseconds = std::chrono::duration<int, MicrosecondTickPeriod>;
    Microseconds microseconds(5000); // 5000 microseconds
    std::cout << microseconds.count() << " microseconds" << std::endl;

	using Minutes = std::chrono::duration<float, MicrosecondTickPeriod>;
	Minutes minutes(11.5);
	std::cout << minutes.count() << " minutes" << std::endl;

	std::cout << "millisecond : " << std::chrono::milliseconds::period::num 
		<< "/" << std::chrono::milliseconds::period::den << "s\n";

	return 0;
}
