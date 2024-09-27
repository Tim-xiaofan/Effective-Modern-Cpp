#include <iostream>
#include <atomic>
#include <future>
#include <mutex>
#include <vector>
#include <cassert>

/**
  spinlock 的改进版
  时间公平：先到先得 FIFO
  避免线程饥饿
 */
class ticketlock
{
    public:
        ticketlock(): turn_(0), ticket_(0) {}

        void lock() 
        {
            // 取号
            auto me = ticket_.fetch_add(1, std::memory_order_relaxed); 
            // 轮到自己的号
            while (me != turn_.load(std::memory_order_acquire)); // (1) 
        }

        void unlock() 
        {
            auto cur = turn_.load(std::memory_order_relaxed);
            // 叫下一个号
            turn_.store(cur + 1, std::memory_order_release); //(2) synchronizes-with (1)
        }

    private:
        std::atomic<int> turn_;
        std::atomic<int> ticket_;
};

namespace
{
constexpr int LOOPS = 10000 * 100;

int data = 0;
ticketlock data_lock;

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
