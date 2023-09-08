#include <iostream>
#include <chrono>
#include <future>
#include <thread>
#include <random>
#include <vector>
#include <cassert>
#include <mutex>
#include <fstream>

//#define DEBUG

namespace
{

#ifdef DEBUG
std::mutex iomutex;
#endif

inline std::chrono::steady_clock::time_point now()
{
	return std::chrono::steady_clock::now();
}

void calc_delay(std::chrono::steady_clock::time_point created_time)
{
	auto duration = now() - created_time;
	std::cout << "thread-" << std::this_thread::get_id()
		<< " delay{count=" << duration.count()
		<< ", numerator=" << decltype(duration)::period::num
		<< ", denominator=" << decltype(duration)::period::den << "}\n";
}

std::string create_random_string(int size)
{
	static const char characters[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890";
	static thread_local std::random_device rd{}; // avoid race
	static thread_local std::mt19937 generator(rd()); // avoid race
	std::uniform_int_distribution<int> distributor(0, sizeof(characters) - 1);
	std::string result;
	for(int i = 0; i < size; ++i)
	{
		result += characters[distributor(generator)];
	}
	return result;
}

std::string create_random_string_parallel(int size)
{
	const int n = std::thread::hardware_concurrency();
	assert(n > 0);
	assert(size > n);
	std::vector<std::future<std::string>> futs;
	int steplen = size / n;
	auto f = [](int len) {
#ifdef DEBUG
		{
			std::lock_guard<std::mutex> lock(iomutex);
			std::cout << "thread-" << std::this_thread::get_id() << std::endl;
		}
#endif
		return create_random_string(len);
	};
	for(int i = 0; i < n - 1; i++)
	{
		auto fut = std::async(std::launch::async, f, steplen); 
		futs.emplace_back(std::move(fut));
	}
	steplen += size % steplen;
	auto fut = std::async(std::launch::async, f, steplen);
	futs.emplace_back(std::move(fut));

	std::string result;
	for(auto& fut: futs)
	{
		result += fut.get();
	}
	assert(result.size() == (size_t)size);
	return result;
}

}

int main()
{
	std::cout << std::thread::hardware_concurrency() << " concurrence thread are supported\n";
	std::cout << "main thread: " << std::this_thread::get_id() << std::endl;

	std::future<void> delay_eliminate;
	{// 延长future生命周期，防止在此析构导致的延迟，因为not ready
		delay_eliminate = std::async([](){
					std::this_thread::sleep_for(std::chrono::milliseconds(550));
					std::cout << __PRETTY_FUNCTION__  <<
					": " << std::this_thread::get_id() << std::endl;}); 
		std::cout << "\n\n";
	}

	{// std::launch::async	the task is executed on a different thread, potentially by creating and launching it first
		std::async(std::launch::async, calc_delay, now());
		std::cout << "\n\n";
	}
	 
	// std::launch::deferred	the task is executed on the calling thread the first time its result is requested (lazy evaluation)
	{
		auto fut = std::async(std::launch::deferred, calc_delay, now());
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		fut.wait();
		std::cout << "\n\n";
	}

	//
	{
		std::fstream fout("n_timedecrease.csv", std::ios::out|std::ios::trunc);
		fout << "n, timedecrease\n";
		for(int i = 1; i <= 100; ++i)
		{
			const int n = 100 * 10000 * i;
			auto start = now();
			create_random_string(n);
			auto duration = now() - start;
			//std::cout << "create_random_string: duration{count=" << duration.count()
			//	<< ", numerator=" << decltype(duration)::period::num
			//	<< ", denominator=" << decltype(duration)::period::den << "}\n";

			start = now();
			create_random_string_parallel(n);
			auto duration1 = now() - start;
			//std::cout << "create_random_string_parallel: duration{count=" << duration1.count()
			//	<< ", numerator=" << decltype(duration1)::period::num
			//	<< ", denominator=" << decltype(duration1)::period::den << "}\n";
			double d = (duration.count() - duration1.count()) * 1.0 / duration.count();
			std::cout << "for n=" << n << ", time decrease: " << d * 100 << "%\n"; 
			fout << n << ", " << d << "\n"; 
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}

	delay_eliminate.wait();
	return 0;
}
