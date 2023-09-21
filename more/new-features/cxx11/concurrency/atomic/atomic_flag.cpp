// std::atomic_flag 是一个原子的布尔类型，也是唯一保证 lock-free 的原子类型
#include <atomic>
#include <iostream>
#include <future>
#include <mutex>
#include <vector>
#include <chrono>

namespace
{

class spinlock
{
	public:
		spinlock() : lock_(ATOMIC_FLAG_INIT) {}

		void lock()
		{
			while(lock_.test_and_set(std::memory_order_acquire));
		}

		void unlock()
		{
			lock_.clear(std::memory_order_release);
		}

	private:
		std::atomic_flag lock_;
};

spinlock iomutex;

std::chrono::steady_clock::duration work_thread(int id)
{
	auto start = std::chrono::steady_clock::now();
	for(int i = 0; i < 100; ++i)
	{
		std::lock_guard<spinlock> lock(iomutex);
		std::cout << "thread-" << id << ": " << i << std::endl;
	}
	return std::chrono::steady_clock::now() - start;
}

}

int main()
{
	{
		std::atomic_flag x = ATOMIC_FLAG_INIT;
		x.clear(std::memory_order_release);
		bool y = x.test_and_set(); // 将状态设为 true 且返回之前的值
		(void) y;
	}

	{
		std::vector<std::future<typename std::chrono::steady_clock::duration>> futs;
		const int n = std::thread::hardware_concurrency();
		for(int i = 0; i < n; ++i)
		{
			futs.emplace_back(std::async(work_thread, i));
		}
		for(auto& f: futs)
		{
			f.wait();
		}
		for(int i = 0; i < n; ++i)
		{
			std::cout << "thread-" << i  << ": duration= "<< futs[i].get().count() << std::endl;
		}
	}
	return 0;
}
