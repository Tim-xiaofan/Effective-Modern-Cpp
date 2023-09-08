#include <iostream>
#include <cmath>
#include <thread>
#include <future>
#include <functional>
#include <chrono>
#include <cassert>
 
namespace
{
// unique function to avoid disambiguating the std::pow overload set
int f(int x, int y) { return std::pow(x,y); }
 
void task_lambda()
{
    std::packaged_task<int(int,int)> task([](int a, int b) {
        return std::pow(a, b); 
    });
    std::future<int> result = task.get_future();
 
    task(2, 9);
 
    std::cout << "task_lambda:\t" << result.get() << '\n';
}
 
void task_bind()
{
    std::packaged_task<int()> task(std::bind(f, 2, 11));
    std::future<int> result = task.get_future();
 
    task();
 
    std::cout << "task_bind:\t" << result.get() << '\n';
}
 
void task_thread()
{
    std::packaged_task<int(int,int)> task(f);
    std::future<int> result = task.get_future();
 
    std::thread task_td(std::move(task), 2, 10);
 
    std::cout << "task_thread:\t" << result.get() << '\n';
    task_td.join();
}

void countdown(int from, int to)
{
	std::cout << "countdown [" << from << ", " << to << "] ";
	for(; from != to; --from)
	{
		std::cout << '.';
		std::flush(std::cout);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	std::cout << std::endl;
}

}
 
int main()
{
	{ // Base Usage
		task_lambda();
		task_bind();
		task_thread();
		std::cout << "\n\n";
	}

	{
		std::packaged_task<void()> task(std::bind(countdown, 10, 0));
		auto fut = task.get_future();
		auto start = std::chrono::system_clock::now();
		std::thread t(std::move(task));
		fut.wait();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
		std::cout << "duration: " << duration.count() << "ms\n";
		t.join();
		std::cout << "\n\n";
	}

	// std::packaged_task::valid 
	// true if *this has a shared state, false otherwise
	{
		std::packaged_task<void()> task1;
		std::cout << "default constructor: task1.valid()=" << std::boolalpha << task1.valid() << std::endl;
		assert(task1.valid() == false);

		std::packaged_task<void()> task2([](){ std::cout << "task2: " << __PRETTY_FUNCTION__ << std::endl; });
		std::cout << "normal constructor: task2.valid()=" << task2.valid() << std::endl;
		assert(task2.valid() == true);

		auto fut = task2.get_future();
		std::thread t(std::move(task2));
		fut.wait();
		std::cout << "after moving to thread: task2.valid()=" <<task2.valid() << std::endl;
		assert(task2.valid() == false);
		t.join();

		std::packaged_task<void()> task3([](){ std::cout << "task3: " << __PRETTY_FUNCTION__ << std::endl; });
		assert(task3.valid() == true);
		auto fut1 = task3.get_future();
		task3();
		fut1.get();
		assert(task3.valid() == true);
		try {
			task3();
		} catch (const std::future_error& e) {
			std::cout << e.what() << std::endl;
		}
		task3.reset();
		assert(task3.valid() == true);
		std::cout << "after reseting to thread: task3.valid()=" << task3.valid() << std::endl;

		task3();
		std::cout << "\n\n";
	}
}
