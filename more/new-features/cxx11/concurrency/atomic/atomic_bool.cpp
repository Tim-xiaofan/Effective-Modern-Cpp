#include <atomic>
#include <iostream>
#include <future>
#include <mutex>
#include <vector>
#include <chrono>

namespace
{

static_assert(ATOMIC_BOOL_LOCK_FREE == 2);

class spinlock
{
	public:
		spinlock() : lock_(false) {}

		void lock()
		{
			bool expected = false;
			while(!lock_.compare_exchange_weak(expected,
							true,
							std::memory_order_acquire,
							std::memory_order_relaxed))
			{
				expected = false;
			}
		}

		void unlock()
		{
			lock_.store(false, std::memory_order_release);
		}

	private:
		std::atomic_bool lock_;
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
			std::cout << "thread-" << i  << ": duration= "
				<< futs[i].get().count() << std::endl;
		}
	}
	return 0;
}
