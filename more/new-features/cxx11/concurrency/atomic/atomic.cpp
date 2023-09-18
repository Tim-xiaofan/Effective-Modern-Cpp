// C++11 支持的原子操作
#include <iostream>
#include <atomic>
#include <future>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <cassert>
#include <memory>
#include <type_traits>

namespace
{

std::atomic_bool run{true};
std::mutex iomutex;

void work_thread(int id)
{
	auto start = std::chrono::steady_clock::now();
	while(run)
	{
	}
	auto end = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
	std::lock_guard<std::mutex> lock(iomutex);
	std::cout << "thread-" << id << " worked " << duration.count() << "ms\n";
}
		
struct A
{
	int a[100];
};

struct B { int x, y; };

enum {CLOSED, OPEN};
static_assert(CLOSED == 0);
static_assert(OPEN == 1);

}

int main(void)
{
	const int n = std::thread::hardware_concurrency();
	// basic usgae
	{
		assert(run.is_lock_free());
		std::vector<std::future<void>> futs;
		for(int i = 0; i < n; ++i)
		{
			auto fut = std::async(work_thread, i);
			futs.emplace_back(std::move(fut));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		run = false;
		for(auto& fut: futs)
		{
			fut.get();
		}
		std::cout << "All threads done.\n";
	}

	{ // is_lock_free
		std::atomic<A> a;
		std::atomic<B> b;
		std::atomic<A*> c;
		assert(!a.is_lock_free());
		assert(b.is_lock_free());
		assert(c.is_lock_free());
	}

	{ // fetch_
		std::atomic<int> a{0};
		// atomically adds the argument to the value stored in the atomic object and obtains the value held previously
		int prev = a.fetch_add(1);
		assert(prev == 0);
		prev = a.fetch_sub(1);
		assert(prev == 1);
	}
	std::cout << "All done.\n";
	return 0;
}
