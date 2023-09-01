#include <iostream>
#include <chrono>
#include <type_traits>
#include <random>

/**
  chrono中有三种时钟：system_clock，steady_clock和high_resolution_clock。每一个clock类中都有确定的time_point, duration, Rep, Period类型。

  system_clock是不稳定的。因为时钟是可调的，即这种是完全自动适应本地账户的调节。
  这种调节可能造成的是，首次调用now()返回的时间要早于上次调用now()所返回的时间，这就违反了节拍频率的均匀分布.
  Class std::chrono::system_clock represents the system-wide real time wall clock
  
  稳定闹钟对于超时的计算很重要，所以C++标准库提供一个稳定时钟std::chrono::steady_clock
  This clock is not related to wall clock time (for example, it can be time since last reboot), and is most suitable for measuring intervals
  
  std::chrono::high_resolution_clock 是标准库中提供的具有最小节拍周期(因此具有最高的精度的时钟)。
  It may be an alias of std::chrono::system_clock or std::chrono::steady_clock, or a third, independent clock.
 */

namespace
{
template<typename Rep, typename Period>
std::ostream& operator<<(std::ostream& os, const std::chrono::duration<Rep, Period>& dur)
{
	using T = std::chrono::duration<Rep, Period>;
	return os << "{count: " << dur.count()
		<< ", numerator=" << T::period::num
		<< ", denominator=" << T::period::den
		<< "}";
}
template<typename Clock>
std::string _ctime(const std::chrono::time_point<Clock, typename Clock::duration>& tp)
{
	char buf[50] = {'\0'};
	time_t tm = Clock::to_time_t(tp);
	ctime_r(&tm, buf);
	return std::string(buf);
}
std::string createRandomString(int size)
{
	static const char characters[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890";
	std::random_device rd{};
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> distributor(0, sizeof(characters)-1);
	std::string result;
	for(int i = 0; i < size; ++i)
	{
		result += characters[distributor(generator)];
	}
	return result;
}
}

int main()
{
	{ // 精度测试
		std::cout << "system_clock{numerator=" << std::chrono::system_clock::period::num
			<< ",denominator=" << std::chrono::system_clock::period::den << "}\n";
		std::cout << "steady_clock{numerator=" << std::chrono::steady_clock::period::num
			<< ",denominator=" << std::chrono::steady_clock::period::den << "}\n";
		std::cout << "high_resolution_clock{numerator=" << std::chrono::high_resolution_clock::period::num
			<< ",denominator=" << std::chrono::high_resolution_clock::period::den << "}\n";
		std::cout << "\n\n";
	}

	{ // 静态成员函数  static time_point now() noexcept 用于获取系统的当前时间。
		std::chrono::time_point<std::chrono::system_clock, typename std::chrono::system_clock::duration> tp1 = std::chrono::system_clock::now();
		auto tp2 = (std::chrono::steady_clock::now());
		auto tp3 = (std::chrono::high_resolution_clock::now());
		std::cout << "tp1 to epoch: " << tp1.time_since_epoch() << std::endl; 
		std::cout << "tp2 to epoch: " << tp2.time_since_epoch() << std::endl; 
		std::cout << "tp3 to epoch: " << tp3.time_since_epoch() << std::endl; 
		std::cout << "\n\n";
	}

	{ // 使用场景
		// 1.获取当前日期时间，以epoch为起点
		std::chrono::time_point<std::chrono::system_clock, typename std::chrono::system_clock::duration> now = std::chrono::system_clock::now();
		std::cout << "Calendar time: " << _ctime(now);

		// 2.从time_t创建时间点
		auto tp = std::chrono::system_clock::from_time_t(114514);
		std::cout << "Calendar time: " << _ctime(tp);

		// 3.获取程序执行时间
		std::chrono::time_point<std::chrono::steady_clock, typename std::chrono::steady_clock::duration> start = std::chrono::steady_clock::now();
		for(int i = 0; i < 10000; ++i)
		{
			createRandomString(i % 4096);
		}
		std::chrono::time_point<std::chrono::steady_clock, typename std::chrono::steady_clock::duration> end = std::chrono::steady_clock::now();
		auto duration = end-start;
		std::chrono::microseconds duration1 = std::chrono::duration_cast<std::chrono::microseconds>(duration);
		std::cout << "Excutation duration: " << duration << std::endl;
		std::cout << "Excutation duration1: " << duration1 << std::endl;
	}
	return 0;
}
