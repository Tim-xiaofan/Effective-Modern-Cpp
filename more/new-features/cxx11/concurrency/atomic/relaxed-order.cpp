// C++11 relaxed-order
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

	std::cout << "All done.\n";
	return 0;
}
