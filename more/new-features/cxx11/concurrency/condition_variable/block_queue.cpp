#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <queue>
#include <random>
#include <vector>
#include <cassert>
#include <string>
#include <sstream>
#include <atomic>

namespace
{

template<typename T>
class block_queue
{
	public:
		block_queue(int capacity = 8): capacity_(capacity){}
		
		bool enqueue(const T& e)
		{
			std::unique_lock<std::mutex> lock(mutex_);
			cv_not_full_.wait(lock, [this](){ return queue_.size() < (size_t)capacity_; });
			if(lock)
			{
				queue_.push(e);
				lock.unlock();
				cv_not_empty_.notify_one();
				return true;
			}
			else
			{
				return false;
			}
		}

		bool dequeue(T& e)
		{
			std::unique_lock<std::mutex> lock(mutex_);
			cv_not_empty_.wait(lock, [this]{ return !queue_.empty(); });
			if(lock)
			{
				e = queue_.front();
				queue_.pop();
				lock.unlock();
				cv_not_full_.notify_one();
				return true;
			}
			else
			{
				return false;
			}
		}

	private:
		mutable std::mutex mutex_;
		std::condition_variable cv_not_full_;
		std::condition_variable cv_not_empty_;
		std::queue<T> queue_;
		const int capacity_;
};

block_queue<uint64_t> queue;
std::atomic<uint64_t> e{0};
enum role {CUSTOMER, PRODUCER};
std::mutex iomutex;

std::string get_name(void)
{
	static thread_local const std::string s = "thread-";
	std::ostringstream oss;
	oss << s << std::this_thread::get_id();
	return oss.str();
}

void work_thread(role r)
{
	static thread_local std::random_device seed{};
	static thread_local std::mt19937 generator(seed());
	std::uniform_int_distribution<int> distributor(100, 400);
	const std::string name = get_name(); 
	{
		std::lock_guard<std::mutex> lock(iomutex);
		std::cout << name << " runing as a "  << (r == CUSTOMER ? "customer":"producer") << std::endl;
	}
	if(r == CUSTOMER)
	{
		uint64_t e;
		while(queue.dequeue(e))
		{
			{
				std::lock_guard<std::mutex> lock(iomutex);
				std::cout << name << " dequeue: " << e << std::endl;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(distributor(generator)));
		}
	}
	else
	{
		while(queue.enqueue(e))
		{
			{
				std::lock_guard<std::mutex> lock(iomutex);
				std::cout << name << " enqueue: " << e << std::endl;
			}
			++e;
			std::this_thread::sleep_for(std::chrono::milliseconds(distributor(generator)));
		}
	}
}

}//namespace

int main(void)
{
	const int n = std::thread::hardware_concurrency();
	assert(n > 1);
	std::vector<std::future<void>> futs;
	for(int i = 0; i < n; ++i)
	{
		auto fut = std::async(work_thread, (i % 2) ?  CUSTOMER: PRODUCER);
		futs.emplace_back(std::move(fut));
	}
	for(auto& fut: futs)
	{
		fut.wait();
	}
	return 0;
}
