#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <future>
#include <numeric>
#include <vector>
#include <stdexcept>
#include <exception>

/**
  1) base template
  2) non-void specialization, used to communicate objects between threads
  3) void specialization, used to communicate stateless events
  The class template std::promise provides a facility to store a value or an exception that is later acquired asynchronously via a std::future object created by the std::promise object. Note that the std::promise object is meant to be used only once.
  只能使用一次
  std::promise的作用就是提供一个不同线程之间的数据同步机制，它可以存储一个某种类型的值
 */

namespace
{
void accumulate(std::vector<int>::const_iterator begin,
			std::vector<int>::const_iterator end,
			std::promise<int> accumulate_promise)
{
	int sum = std::accumulate(begin, end, 0);
	accumulate_promise.set_value(sum); // Notify future
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void do_work(std::promise<void> barrier)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	barrier.set_value();
}

void do_work_exception(std::promise<int> p)
{
	p.set_exception(std::make_exception_ptr(std::invalid_argument(__PRETTY_FUNCTION__)));
}
}

int main()
{
	{
		// Demonstrate using promise<int> to transmit a result between threads
		std::promise<int> accumulate_promise;
		// get_future 来获取与该 promise 对象相关联的 future 对象，调用该函数之后，两个对象共享相同的共享状态(shared state)
		auto fut = accumulate_promise.get_future();
		std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9};
		// 只能move，禁止拷贝
		std::thread t(accumulate, v.begin(), v.end(), std::move(accumulate_promise));

		// future::get() will wait until the future has a valid result and retrieves it.
		// Calling wait() before get() is not needed
		// accumulate_future.wait();  // wait for result
		std::cout << "Result: " << fut.get() << std::endl;
		auto start = std::chrono::system_clock::now();
		t.join();
		auto end = std::chrono::system_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
		std::cout << "duration{count=" << duration.count()
			<< ", numerator=" << decltype(duration)::period::num
			<< ", denominator=" << decltype(duration)::period::den << "}\n";
		std::cout << "\n\n";
	}

	{
		// Demonstrate using promise<void> to signal state between threads
		std::promise<void> barrier;
		auto fut = barrier.get_future();
		try {
			auto fut1 = barrier.get_future(); // error: 只能从 promise 共享状态获取一个futrue对象 
		} catch (const std::future_error& e) {
			std::cout << e.what() << std::endl;
		}
		std::thread t(do_work, std::move(barrier));
		fut.wait();
		t.join();
		std::cout << "\n\n";
	}

	// std::promise::set_exception
	{
		std::promise<int> barrier;
		auto fut = barrier.get_future();
		std::thread t(do_work_exception, std::move(barrier));
		try {
		  fut.get();
		} catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		t.join();
	}
	return 0;
}
