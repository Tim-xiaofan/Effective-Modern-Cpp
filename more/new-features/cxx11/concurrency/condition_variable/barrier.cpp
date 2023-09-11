#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <cassert>
#include <future>
#include <vector>
#include <random>

namespace
{

class barrier 
{
	public:
	    explicit barrier(int count) : count_(count), current_count_(count), generation_(0) {}
	
	    void wait() {
	        std::unique_lock<std::mutex> lock(mutex_);
	        int gen = generation_;
	
	        if (--current_count_ == 0) {
	            // The last thread to arrive resets the barrier for the next generation.
	            current_count_ = count_;
	            ++generation_;
	            condition_.notify_all();
	        } else {
	            // Wait until the last thread arrives and resets the barrier.
	            condition_.wait(lock, [this, gen] { return gen != generation_; });
	        }
	    }

		int generation() const
		{
			std::lock_guard<std::mutex> lock(mutex_);
			return generation_;
		}
	
	private:
	    int count_;
	    int current_count_;
	    int generation_;
	    mutable std::mutex mutex_;
	    std::condition_variable condition_;
};

const int n = std::thread::hardware_concurrency();
barrier b(n);
std::mutex iomutex;

void work_thread(int id)
{
	static thread_local std::random_device seed{};
	static thread_local std::mt19937 generator(seed());
	std::uniform_int_distribution<int> distributor(100, 900);
	std::this_thread::sleep_for(std::chrono::milliseconds(distributor(generator)));
	{
		std::lock_guard<std::mutex> lock(iomutex);
		std::cout << "thread-" << id << " arrived\n";
	}
	b.wait();
	{
		auto now = std::chrono::system_clock::now();
		std::lock_guard<std::mutex> lock(iomutex);
		std::cout << "thread-" << id << " set off at " << std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count() << "us\n";
	}
}

}//namespace

int main()
{
	assert(n > 1);
	for(int generation = 0; generation < 3; generation++)
	{
		std::cout << "generation " << generation << std::endl;
		std::vector<std::future<void>> futs;
		for(int i = 0; i < n; ++i)
		{
			auto fut = std::async(work_thread, i);
			futs.emplace_back(std::move(fut));
		}
		for(auto& fut: futs)
		{
			fut.wait();
		}
		std::cout << "\n\n";
	}
	return 0;
}
