#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <future>

namespace
{

void thread_fn(std::chrono::system_clock::time_point& tp)
{
	tp = std::chrono::system_clock::now(); //设置结果
}

}

int main()
{
	{ //获取线程执行结果
		std::chrono::system_clock::time_point tp; // 创建变量
		std::thread t(thread_fn, std::ref(tp)); // 传递变量引用
		t.join(); // join
		// 得到结果
		time_t tm = std::chrono::system_clock::to_time_t(tp);
		std::cout << "calendar: " << ctime(&tm);
	}

	{ // async: 更方便的获取线程执行结果
		std::future<std::chrono::system_clock::time_point> fut = std::async(std::launch::async, 
					[]() { return std::chrono::system_clock::now();});
		// 得到结果
		std::chrono::system_clock::time_point tp = fut.get();
		time_t tm = std::chrono::system_clock::to_time_t(tp);
		std::cout << "calendar: " << ctime(&tm);
	}

	{ // std::future 用于获取异步执行的结果，由promise, packaged_task, async等创建
		auto is_prime = [](unsigned long long x) {
			for(unsigned long long i = 2; i < x; ++i){
				if(x % i == 0) return false;
			}
			return true;
		};
		std::cout << "checking";
		auto fut = std::async(is_prime, 444444443);
		while(fut.wait_for(std::chrono::milliseconds(100)) == std::future_status::timeout)
		{
			std::cout << '.';
			std::flush(std::cout);
		}
		std::cout << "\n444444443 is " << (fut.get()?"prime":"not prime") << ".\n";
	}
	return 0;
}
