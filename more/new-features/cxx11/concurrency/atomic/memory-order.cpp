// C++11 memory-order
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
#include <string>

namespace A
{

// CPU 重排指令
bool flag = 0;
int data = 0;

void thread1()
{
	flag = 1; // S1
	//__sync_synchronize();
	data = 3; // S2(可能先于S1执行完毕)
}

void thread2()
{
	while(!flag);
	assert(data == 3); // assert可能失败也可能成功
}

}

namespace
{

// Relaxed ordering
// 宽松顺序仅保证原子操作自身的原子性，对其他读写操作不做任何同步，亦无顺序上的限制。
// 因此，它们不是同步操作，仅保证原子变量上读写操作的原子性，以及各个原子变量自身修改顺序的一致性
//（对于同一个变量的两次修改，虽然顺序不一定，但是所有其他线程观察到的修改顺序都是相同的）。
std::atomic<int> cnt = {0};
void f()
{
	for (int n = 0; n < 1000; ++n)
	  cnt.fetch_add(1, std::memory_order_relaxed);
}

// Release-Acquire ordering
// If an atomic store in thread A is tagged memory_order_release, an atomic load in thread B from the same variable 
// is tagged memory_order_acquire, and the load in thread B reads a value written by the store in thread A,
// then the store in thread A synchronizes-with the load in thread B

// All memory writes (including non-atomic and relaxed atomic) that happened-before the atomic store from the point of 
// view of thread A, become visible side-effects in thread B. That is, once the atomic load is completed, thread B is
// guaranteed to see everything thread A wrote to memory. This promise only holds if B actually returns the value that A stored, or a value from later in the release sequence.

std::atomic<int> data{0}; // atomic
std::string* pstr = nullptr; // non-atomic
std::atomic<bool> guard{false}; // guard(release/acquire)

void producer()
{
	data.store(1, std::memory_order_relaxed); //S1
	pstr = new std::string("release"); //S2
	guard.store(true, std::memory_order_release); // 保证S1,S2的内存写都发生在store之前
}

void customer()
{
	while(!guard.load(std::memory_order_acquire)); // 如果读到store的值，保证S1和S2的内存写在此线程可见
	assert(data.load(std::memory_order_relaxed) == 1); // never fires
	assert(*pstr == "release"); // never fires
}

// The following example demonstrates transitive(传递性) release-acquire ordering across three threads, using a release sequence.
std::vector<int> v{};
std::atomic<int> flag{0};
void thread1()
{
	v.push_back(42);
	flag.store(1, std::memory_order_release);
}

void thread2()
{
	int expected = 1;
	// memory_order_relaxed is okay because this is an RMW,
    // and RMWs (with any ordering) following a release form a release sequence
	while(!flag.compare_exchange_strong(expected, 2, std::memory_order_relaxed))
	{
		expected = 1;
	}
}

void thread3()
{
	while(flag.load(std::memory_order_acquire) != 2);
	assert(v[0] == 42); // never fires
}


// Release-Consume ordering


}

int main(void)
{
	const int n = std::thread::hardware_concurrency();
	{
		std::thread t1(A::thread1);
		std::thread t2(A::thread2);
		t1.join();
		t2.join();
	}

	{
		std::vector<std::thread> v;
		for (int i = 0; i < n; ++i)
		  v.emplace_back(f);
		for (auto& t : v)
		  t.join();
		assert(cnt.load(std::memory_order_relaxed) == n * 1000);
		std::cout << "Final counter value is " << cnt << '\n';
	}

	{
		std::thread t1(producer), t2(customer);
		t1.join();
		t2.join();
	}

	{
		std::thread t1(thread1), t2(thread2), t3(thread3);
		t1.join();
		t2.join();
		t3.join();
	}
	std::cout << "All done.\n";
	return 0;
}
