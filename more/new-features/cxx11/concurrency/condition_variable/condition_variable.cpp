#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <chrono>

namespace
{

bool ready = false;
std::mutex mutex;
std::condition_variable cv;

void work_thread()
{
	std::cout << "wait ready true\n";
	std::unique_lock<std::mutex> lock(mutex);
	cv.wait(lock, []{ return ready == true; });
	std::cout << "ready true\n";
}

}//namespace

int main()
{
	auto fut = std::async(work_thread);
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		std::lock_guard<std::mutex> lock(std::mutex);
		ready = true;
	}
	cv.notify_one();
	fut.wait();

	{
		std::mutex mutex;
		std::condition_variable cv;
		std::unique_lock<std::mutex> lock(mutex);
		bool rv = cv.wait_for(lock, std::chrono::milliseconds(1000), [](){ return false; });
		if(!rv)
		{
			std::cerr << "wait_for timeout\n";
		}
	}

	return 0;
}
