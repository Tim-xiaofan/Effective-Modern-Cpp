#include "ticketlock.h"
#include "spinlock.h"
#include <cassert>
#include <future>
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <functional>

namespace
{

constexpr int loops = 100000;
int data = 0;

template<typename Mutex>
void work_thread(Mutex& mutex)
{
    for(int i = 0; i < loops; ++i)
    {
        std::lock_guard<Mutex> lock(mutex);
        ++data;
    }
}

template<typename Mutex>
void test_lock(void)
{
    const int cpus = std::thread::hardware_concurrency();
    Mutex mutex;
    std::vector<std::future<void>> futs;
    for(int i = 0; i < cpus; ++i)
    {
        futs.emplace_back(std::async(work_thread<Mutex>, std::ref(mutex)));
    }
    for(auto& f: futs)
    {
        f.wait();
    }
    assert(data == loops * cpus);
    data = 0;
}

}

int main()
{
    test_lock<ticketlock>();
    test_lock<spinlock>();
    std::cout << "All tests passed." << std::endl;
    return 0;
}
