#include <iostream>
#include <atomic>
#include <future>
#include <mutex>
#include <vector>
#include <cassert>

class spinlock
{
    public:
        spinlock(): flag_(ATOMIC_FLAG_INIT) {}

        void lock() 
        {
            while(flag_.test_and_set(std::memory_order_acquire));
        }

        void unlock() 
        {
            flag_.clear(std::memory_order_release);
        }

    private:
        std::atomic_flag flag_;
};

namespace
{
constexpr int LOOPS = 10000 * 100;

int data = 0;
spinlock data_lock;

void thread_fn(void) 
{
    for (int i = 0; i < LOOPS; ++i) {
        std::lock_guard lock(data_lock); // (1)
        ++data; // (2)
    } // (3) unlock  synchronizes-with (1) -> (3) happend before (2)
}

}

int main(void)
{
    int cpus = std::thread::hardware_concurrency();
    assert(cpus > 1);

    {
        std::vector<std::future<void>> futs;
        for (int i = 0; i < cpus; ++i) {
            futs.emplace_back(std::async(thread_fn));
        }
    }

    assert(data == LOOPS * cpus);
    std::cout << "All tests passed: " << cpus << "\n";
    return 0;
}
