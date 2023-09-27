#include <atomic>
#include <iostream>
#include <future>
#include <mutex>
#include <vector>
#include <chrono>

namespace
{

static_assert(ATOMIC_BOOL_LOCK_FREE == 2);

// 参考 DPDK rte_ticketlock.h
class ticketlock
{ // 时间公平：先到先获得锁, 避免线程饥饿(FIFO)
	public:
		ticketlock() : turn_(0), ticket_(0) {}

		void lock()
		{
			int me = ticket_.fetch_add(1, std::memory_order_relaxed);
			while(me != turn_.load(std::memory_order_acquire))
			{
			}
		}

		void unlock()
		{
			int new_turn = turn_.load(std::memory_order_relaxed); 
			turn_.store(new_turn + 1, std::memory_order_release);
		}

	private:
		std::atomic_int turn_;
		std::atomic_int ticket_;
};

ticketlock iomutex;

std::chrono::steady_clock::duration work_thread(int id)
{
	auto start = std::chrono::steady_clock::now();
	for(int i = 0; i < 100; ++i)
	{
		std::lock_guard<ticketlock> lock(iomutex);
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

	{
		ticketlock mutex;
		mutex.lock();
		mutex.lock();
	}
	std::cout << "All done." << std::endl;
	return 0;
}
