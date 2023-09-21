#include <iostream>
#include <future>
#include <chrono>
#include <vector>
#include <atomic>
#include <chrono>
#include <random>
#include <mutex>
#include <cassert>

namespace
{

class barrier
{
	public:
		barrier(int count):
			count_(count),
			not_arrived_(count),
			generation_(0)
		{}

		void wait()
		{
			int gen = generation_.load(std::memory_order_relaxed);
			if(not_arrived_.fetch_sub(1, std::memory_order_relaxed) == 1)
			{
				not_arrived_.store(count_, std::memory_order_relaxed);
				generation_.store(gen + 1, std::memory_order_release);
			}
			else
			{
				while(generation_.load(std::memory_order_acquire) == gen)
				{
					 std::this_thread::yield();
				}
			}
		}

	private:
		const int count_;
		std::atomic<int> not_arrived_;
		std::atomic<int> generation_;
};

const int n = std::thread::hardware_concurrency();
constexpr int stages = 10;
std::vector<int> results;
std::mutex iomutex;
barrier my_barrier(n);

void work_thread(int id)
{
	static thread_local std::random_device seed{};
	static thread_local std::mt19937 generator(seed());
	static thread_local std::uniform_int_distribution<int> distributor(0, 1000);
	
	// stag1
	for(int s = 0; s < stages; ++s)
	{
		{
			std::lock_guard<std::mutex> lock(iomutex);
			std::cout << "Thread " << id << " is doing stage-" << s << " work." << std::endl;
			results.push_back(s);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(distributor(generator)));
		my_barrier.wait();
	}
}

std::ostream& operator<<(std::ostream& os, std::vector<int>& v)
{
	for(int i: v)
	{
		os << i << ' ';
	}
	return os;
}

}

int main()
{
	std::vector<std::future<void>> futs;
	for(int i = 0; i < n; ++i)
	{
		futs.emplace_back(std::async(work_thread, i));
	}

	for(auto& f: futs)
	{
		f.wait();
	}

	std::vector<int> expected;
	for(int s = 0; s < stages; ++s)
	{
		for(int i = 0; i < n; ++i)
		{
			expected.push_back(s);
		}
	}
	std::cout << "expected: " << expected << std::endl;
	std::cout << "results:  " << results << std::endl;
	assert(expected == results);
	std::cout << "All tests passed\n";
	return 0;
}
