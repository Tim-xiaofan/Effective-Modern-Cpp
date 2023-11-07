#include <iostream>
#include <atomic>
#include <future>
#include <vector>
#include <mutex>
#include <random>
#include <chrono>
#include <cassert>
#include <string>

namespace
{

// 参考 DPDK rte_ticketlock.h
class ticketlock
{ // 时间公平：先到先获得锁, 避免线程饥饿
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

class rwlock
{
	public:
		rwlock(): count_(0){}

		void write_lock()
		{
			int expected = 0;
			while(!count_.compare_exchange_weak(expected,
						-1,
						std::memory_order_acquire,
						std::memory_order_relaxed))
			{
				expected = 0;
			}
		}

		void write_unlock()
		{
			count_.store(0, std::memory_order_release);
		}

		void read_lock()
		{
            bool success = false;
            do
            {
                int expected = count_.load(std::memory_order_relaxed);
                if(expected < 0)
                { // write lock is held
                    continue;
                }
                success = count_.compare_exchange_weak(expected,
                            expected + 1,
                            std::memory_order_acquire,
                            std::memory_order_relaxed);
            }while(!success);
		}

		void read_unlock()
		{
			count_.fetch_sub(1, std::memory_order_release);
		}

	private:
		std::atomic<int> count_; // -1 when W lock held, > 0 when R locks held.
};

int data = 0;
rwlock mutex;
ticketlock mutex1;
std::mutex mutex2;
ticketlock iomutex;
std::atomic<std::uint64_t> number_reading{0};
constexpr std::uint64_t MAX_NUMBER = 10000 * 10000;
constexpr std::uint64_t PER_NUMBER = 100 * 10000;

std::chrono::steady_clock::duration work_thread(int id)
{
	static thread_local std::random_device seed{};
	static thread_local std::mt19937 generator(seed());
	static thread_local std::uniform_int_distribution<int> distributor(0, 1024);

	auto start = std::chrono::steady_clock::now();
	if(id == 0)
	{
		while(number_reading.load(std::memory_order_relaxed) < MAX_NUMBER)
		{
			if(number_reading.load(std::memory_order_relaxed)% PER_NUMBER == 0)
			{
				mutex.write_lock();
				data = distributor(generator);
				mutex.write_unlock();
			}
		}
	}
	else
	{
		while(number_reading.load(std::memory_order_relaxed) < MAX_NUMBER)
		{
			int tmp;
			mutex.read_lock();
			tmp = data;
			mutex.read_unlock();
			(void)tmp;
			number_reading.fetch_add(1, std::memory_order_relaxed);
		}
	}
	auto duration = std::chrono::steady_clock::now() - start;
	std::lock_guard<ticketlock> lock(iomutex);
	std::cout << "thread-" << id << ": duration=" << duration.count() << std::endl;
	return duration;
}

std::chrono::steady_clock::duration work_thread1(int id)
{
	static thread_local std::random_device seed{};
	static thread_local std::mt19937 generator(seed());
	static thread_local std::uniform_int_distribution<int> distributor(0, 1024);

	auto start = std::chrono::steady_clock::now();
	if(id == 0)
	{
		while(number_reading.load(std::memory_order_relaxed) < MAX_NUMBER)
		{
			if(number_reading.load(std::memory_order_relaxed)% PER_NUMBER == 0)
			{
				std::lock_guard<ticketlock> lock(mutex1);
				data = distributor(generator);
			}
		}
	}
	else
	{
		while(number_reading.load(std::memory_order_relaxed) < MAX_NUMBER)
		{
			int tmp;
			{
				std::lock_guard<ticketlock> lock(mutex1);
				tmp = data;
			}
			(void)tmp;
			number_reading.fetch_add(1, std::memory_order_relaxed);
		}
	}
	auto duration = std::chrono::steady_clock::now() - start;
	std::lock_guard<ticketlock> lock(iomutex);
	std::cout << "thread1-" << id << ": duration=" << duration.count() << std::endl;
	return duration;
}

std::chrono::steady_clock::duration work_thread2(int id)
{
	static thread_local std::random_device seed{};
	static thread_local std::mt19937 generator(seed());
	static thread_local std::uniform_int_distribution<int> distributor(0, 1024);

	auto start = std::chrono::steady_clock::now();
	if(id == 0)
	{
		while(number_reading.load(std::memory_order_relaxed) < MAX_NUMBER)
		{
			if(number_reading.load(std::memory_order_relaxed)% PER_NUMBER == 0)
			{
				std::lock_guard<std::mutex> lock(mutex2);
				data = distributor(generator);
			}
		}
	}
	else
	{
		while(number_reading.load(std::memory_order_relaxed) < MAX_NUMBER)
		{
			int tmp;
			{
				std::lock_guard<std::mutex> lock(mutex2);
				tmp = data;
			}
			(void)tmp;
			number_reading.fetch_add(1, std::memory_order_relaxed);
		}
	}
	auto duration = std::chrono::steady_clock::now() - start;
	std::lock_guard<ticketlock> lock(iomutex);
	std::cout << "thread2-" << id << ": duration=" << duration.count() << std::endl;
	return duration;
}

using func_t = std::chrono::steady_clock::duration (*)(int);

}

int main()
{
	const int n = std::thread::hardware_concurrency();
	assert(n > 1);

	double durations[3] = {0, 0, 0};
	func_t funcs[3] = {work_thread, work_thread1, work_thread2};
	const std::string names[] = {"rwlock", "ticketlock", "std::mutex"};

	for(int k = 0; k < 3; ++k)
	{
		std::cout << "*****" << names[k] << std::endl; 
		std::vector<std::future<std::chrono::steady_clock::duration>> futs;
		for(int i = 0; i < n; ++i)
		{
			futs.emplace_back(std::async(funcs[k], i));
		}

		std::chrono::steady_clock::duration duration{0};
		for(auto& fut: futs)
		{
			duration += fut.get();
		}
		std::cout << "\tMAX_NUMBER: " << MAX_NUMBER << std::endl;
		std::cout << "\tPER_NUMBER: " << PER_NUMBER << std::endl;
		std::cout << "\tn:          " << n << std::endl;
		std::cout << "\tduration:   " << duration.count() << std::endl;
		std::cout << "\taverage:    " << duration.count() / n << std::endl;
		std::cout << "\taverage:    " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / n  << "ms" << std::endl;
		number_reading.store(0, std::memory_order_relaxed);
		durations[k] = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() *1.0 /n;
	}
	std::cout << "***statistics\n";
	std::cout << "\tticketlock percentage: " << (durations[1] - durations[0]) * 1.0/ durations[1] * 100  << "%" << std::endl;
	std::cout << "\tstd::mutex percentage: " << (durations[2] - durations[0]) * 1.0/ durations[2] * 100  << "%" << std::endl;
	return 0;
}
