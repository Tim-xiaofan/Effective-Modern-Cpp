#include <atomic>
#include <cassert>
#include <thread>
#include <iostream>
#include <unordered_set>
 
std::atomic<bool> x = {false};
std::atomic<bool> y = {false};
std::atomic<int> z = {0};
 
void write_x()
{
    x.store(true, std::memory_order_seq_cst);
}
 
void write_y()
{
    y.store(true, std::memory_order_seq_cst);
}
 
void read_x_then_y()
{
    while (!x.load(std::memory_order_seq_cst))
        ;
    if (y.load(std::memory_order_seq_cst))
        ++z;
}
 
void read_y_then_x()
{
    while (!y.load(std::memory_order_seq_cst))
        ;
    if (x.load(std::memory_order_seq_cst))
        ++z;
}
 
int main()
{
	std::unordered_set<int> s;
	for(int i = 0; i < 100; ++i)
	{
		std::thread a(write_x);
		std::thread b(write_y);
		std::thread c(read_x_then_y);
		std::thread d(read_y_then_x);
		a.join(); b.join(); c.join(); d.join();
		assert(z.load(std::memory_order_relaxed) != 0); // will never happen
		
		s.insert(z.load(std::memory_order_relaxed));

		y.store(false, std::memory_order_relaxed);
		x.store(false, std::memory_order_relaxed);
		z.store(0, std::memory_order_relaxed);

		// This ensures that all subsequent reads will not be reordered before this point.
		std::atomic_thread_fence(std::memory_order_acquire);
	}
	for(int i: s)
	{
		std::cout << i << ", ";
	}
	std::cout << std::endl;
}
