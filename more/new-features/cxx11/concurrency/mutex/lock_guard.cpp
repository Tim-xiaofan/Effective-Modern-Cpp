#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

namespace
{

#define LOCK

#ifdef LOCK
std::mutex cout_mutex;
#endif

void thread_fn(int id)
{
#ifdef LOCK
	//RAII: 更安全（防止忘记unlock，防止抛出异常未unlock）
	std::lock_guard<std::mutex> lock(cout_mutex);
#endif
	std::cout << "thread-" << id << std::endl; // 无lock可能会出现几个线程输出错位的情况
}

}

int main()
{
	const int n = std::thread::hardware_concurrency();
	std::vector<std::thread> threads;
	for(int i = 0; i < n; ++i)
	{
		threads.emplace_back(std::move(std::thread(thread_fn, i)));
	}

	for(auto& t: threads)
	{
		t.join();
	}
	return 0;
}
