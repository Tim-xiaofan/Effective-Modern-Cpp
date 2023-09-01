#include <iostream>
#include <chrono>
#include <ctime>
#include <cassert>

/*
   template <class Clock, class Duration = typename Clock::duration>
   class time_point;
   std::chrono::time_point 表示一个具体时间，如上个世纪80年代、今天下午3点、火车出发时间等，只要它能用计算机时钟表示。

   第一个模板参数Clock用来指定所要使用的时钟（标准库中有三种时钟，system_clock，steady_clock和high_resolution_clock。见4时钟详解），第二个模板函数参数用来表示时间的计量单位(特化的std::chrono::duration<> )

   时间点都有一个时间戳，即时间原点。chrono库中采用的是Unix的时间戳1970年1月1日 00:00。所以time_point也就是距离时间戳(epoch)的时间长度（duration）。
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
}

namespace A
{
	using years = std::chrono::duration<long long unsigned, std::ratio<60 * 60 * 24 * 365, 1>>; // 未计算平年和闰年
}

int main()
{
	{// constructors
		/**
		  time_point();           //默认构造函数，时间戳作为其值
		  template <class Duration2> time_point (const time_point<clock,Duration2>& tp);  //拷贝构造函数
		  explicit time_point (const duration& dtn); //使用duration构造，就是距离时间戳的时间长度
		 */
		std::chrono::time_point<std::chrono::system_clock> def;
		std::cout << "def to epoch: " << def.time_since_epoch() << std::endl;

		std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> tp(std::chrono::duration<int, std::ratio<1, 1>>(2)); //距离 epoch 2秒
		std::cout << "tp to epoch: " << tp.time_since_epoch() << std::endl;
		std::cout << "\n\n";
	}
	
	{ // 当前时间点距离时间戳的时间长度
		std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> tp(A::years(53));
		//转化为ctime，打印输出时间点
		time_t tt = std::chrono::system_clock::to_time_t(tp);
		std::cout << ctime(&tt);
		std::cout << "\n\n";
	}

	{ 
		// 根据duration计算新的时间点
		auto now = std::chrono::system_clock::now();
		auto later = now + std::chrono::seconds(20); 
		auto before = now - std::chrono::minutes(120);

		// 比较两个时间点
		assert(now == now);
		assert(now < later);
		assert(now > before);
	
		// 时间点之差
		assert(later - now == std::chrono::seconds(20));
		assert(now - later == std::chrono::seconds(-20));
		assert(now - before == std::chrono::minutes(120));
		assert(before - now == std::chrono::minutes(-120));
	}

	return 0;
}
