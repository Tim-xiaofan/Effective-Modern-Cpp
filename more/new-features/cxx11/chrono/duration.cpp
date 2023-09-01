#include <iostream>
#include <chrono>
#include <string>

//template <class Rep, class Period = ratio<1> > // 默认秒级
//class duration;

//Rep表示一种数值类型，用来表示Period的数量，比如int float double。
//Period是ratio类型，用来表示上面所说的单位精度，比如second milisecond。
//chrono中宏定义了许多特例化了的duration：
//就是常见的hours，miniutes，seconds，milliseconds等，使用std::chrono::milliseconds直接使用。

namespace
{

template<typename Rep, typename Period = std::ratio<1>>
void dumpDuration(const std::chrono::duration<Rep, Period>& dur, const std::string& info)
{
	using T = std::chrono::duration<Rep, Period>;
	std::cout << info << "{count: " << dur.count()
		<< ", numerator=" << T::period::num
		<< ", denominator=" << T::period::den
		<< "}\n";
}

}

int main()
{
	{//construct
		/**
		  (1)duration() = default;    //默认构造
		  (2)duration (const duration& dtn);        //(2)(3)拷贝构造
		  (3)template<class Rep2, class Period2> constexpr duration (const duration<Rep2,Period2>& dtn);
		  (4)template<class Rep2> constexpr explicit duration (const Rep2& n);//传递一个某类型（int等）的数值，构造一个时间段
		 */
		std::cout << "construt\n";
		auto def = std::chrono::duration<int>();
		dumpDuration(def, "def");

		auto def1 = std::chrono::duration<int>(def);
		dumpDuration(def1, "def1");

		std::chrono::duration<float> dur = std::chrono::duration<float>(3.14);
		dumpDuration(dur, "dur");

		// 预定义
		std::chrono::milliseconds milliseconds(5000); //毫秒
		dumpDuration(milliseconds, "milliseconds");
		std::chrono::microseconds microseconds(20); //微妙
		dumpDuration(microseconds, "microseconds");
		std::chrono::minutes minutes(60);
		dumpDuration(minutes, "minutes");
		std::cout << "\n\n";
	}

	{// 隐式转换：适用于无截断值
		std::cout << "implicit cast\n";
		std::chrono::minutes minutes(60);
		dumpDuration(minutes, "minutes");
		std::chrono::seconds seconds = minutes; // OK
		dumpDuration(seconds, "to seconds");
		std::chrono::milliseconds milliseconds = minutes; //OK
		dumpDuration(milliseconds, "to milliseconds");

		//std::chrono::hours hours = minutes; // Error
		std::cout << "\n\n";
	}
	
	{ // 显示转换：用于有截断的情况 
		std::cout << "explicit cast\n";
		std::chrono::seconds seconds(65);
		dumpDuration(seconds, "seconds");
		std::chrono::minutes minutes = std::chrono::duration_cast<std::chrono::minutes>(seconds);
		dumpDuration(minutes, "to minutes");
		std::chrono::hours hours = std::chrono::duration_cast<std::chrono::hours>(seconds);
		dumpDuration(minutes, "to hours");
		std::chrono::milliseconds milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(seconds);
		dumpDuration(milliseconds, "to milliseconds");
	}
	return 0;
}
