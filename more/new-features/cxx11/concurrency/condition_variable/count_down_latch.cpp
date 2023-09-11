#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <cassert>
#include <future>
#include <vector>

namespace
{

// 适用于主线程等待其他工作线程结束
class count_down_latch
{
	public:
		count_down_latch(int c = 1): count_(c)
		{
			assert(count_ > 0);
		}

		int count() const
		{
			std::lock_guard<std::mutex> lock(mutex_);
			return count_;
		}

		void count_down(void)
		{
			std::lock_guard<std::mutex> lock_guard(mutex_);
			if(count_ > 0)
			{
				--count_;
			}
			if(count_ == 0)
			{
				cv_.notify_all();
			}
		}

		void wait(void)
		{
			std::unique_lock<std::mutex> lock(mutex_);
			cv_.wait(lock, [this](){ return count_ == 0; });
		}
	
	private:
		std::condition_variable cv_;
		mutable std::mutex mutex_;
		int count_;
};

const int n = std::thread::hardware_concurrency();
count_down_latch cdl(n);
std::mutex iomutex;

void work_thread(int i)
{
	{
		std::lock_guard<std::mutex> lock(iomutex);
		std::cout << "thread-" << std::this_thread::get_id() << std::endl;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(500 * i));
	cdl.count_down();
}

}//namespace

int main()
{
	assert(n > 1);
	std::vector<std::future<void>> futs;
	for(int i = 0; i < n; ++i)
	{
		auto fut = std::async(work_thread, i + 1);
		futs.emplace_back(std::move(fut));
	}
	auto start = std::chrono::steady_clock::now();
	cdl.wait();
	auto end = std::chrono::steady_clock::now();
	{
		std::lock_guard<std::mutex> lock(iomutex);
		auto d = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout << "wait finished: " << d.count() << "ms\n";
	}
	for(auto& fut: futs)
	{
		fut.wait();
	}
	return 0;
}
