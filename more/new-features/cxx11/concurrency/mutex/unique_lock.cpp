#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <utility>

// std::unique_lock 更强大，更灵活的RAII for std::mutex
// The class unique_lock is a general-purpose mutex ownership wrapper allowing 
// deferred locking, time-constrained attempts at locking, recursive locking, 
// transfer of lock ownership, and use with condition variables.

namespace
{

std::mutex m_a, m_b, m_c;
int a, b, c = 1;

void thread_fn(int id)
{
	{   // Note: std::lock_guard or atomic<int> can be used instead
		std::unique_lock<std::mutex> lk(m_a);// Locks the associated mutex by calling m.lock().
		a++;
	}

    {   // Note: see std::lock and std::scoped_lock for details and alternatives
        std::unique_lock<std::mutex> lk_b(m_b, std::defer_lock);
        std::unique_lock<std::mutex> lk_c(m_c, std::defer_lock);
		// Locks the given Lockable objects lock1, lock2, ..., lockn using a deadlock avoidance algorithm to avoid deadlock
        std::lock(lk_b, lk_c);
		// Replaces the value of c with new_value(b+c) and returns the old value of c
        // b = std::exchange(c, b+c); // since c++14
		{
			int tmp = c;
			c = b + c;
			b = tmp;
		}
   }
}

}

int main()
{
	constexpr int n = 12;

	{
		std::vector<std::thread> threads;
		for(int i = 0; i < n; ++i)
		{
			threads.emplace_back(std::move(std::thread(thread_fn, i)));
		}

		for(auto& t: threads)
		{
			t.join();
		}
		std::cout << a << "'th and " << a + 1 << "'th Fibonacci numbers: "
              << b << " and " << c << '\n';
	}
	return 0;
}
