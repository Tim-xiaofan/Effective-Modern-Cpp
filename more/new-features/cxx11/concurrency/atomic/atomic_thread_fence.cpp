#include <atomic>
#include <future>
#include <cassert>
#include <iostream>

namespace
{

std::atomic_bool x{false};
std::atomic_bool y{false};
std::atomic_int z{0};

void f1(void)
{
	x.store(true, std::memory_order_relaxed); // 1 happens-before 2
	std::atomic_thread_fence(std::memory_order_release); // 2 synchronizes-with 3
	y.store(true, std::memory_order_relaxed);
}

void f2(void)
{
	while(!y.load(std::memory_order_relaxed));
	std::atomic_thread_fence(std::memory_order_acquire); // 3 happens-before 4
	if(x.load(std::memory_order_relaxed)) // 4
	{
		++z;
	}
}

};

int main()
{
	{
		auto fut1 = std::async(f1);
		auto fut2 = std::async(f2);
	}
	assert(z.load(std::memory_order_relaxed) != 0); // never fires
	std::cout << "never fires.\n";
	return 0;
}
