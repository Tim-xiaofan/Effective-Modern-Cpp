#include "rwlock.h"
#include "rte_rwlock.h"
#include <cassert>
#include <future>
#include <shared_mutex>
#include <mutex>
#include <string>
#include <vector>
#include <iostream>
#include <functional>

namespace
{

class rte_rwlock_wrapper
{
    public:
        rte_rwlock_wrapper()
        {
            rte_rwlock_init(&mutex_);
        }

        void lock()
        {
            rte_rwlock_write_lock(&mutex_);
        }

        void unlock()
        {
            rte_rwlock_write_unlock(&mutex_);
        }

        void lock_shared()
        {
            rte_rwlock_read_lock(&mutex_);
        }

        void unlock_shared()
        {
            rte_rwlock_read_unlock(&mutex_);
        }

    private:
        rte_rwlock_t mutex_;
};


constexpr int LOOPS = 10000;
const std::vector<std::string> strs{
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
};
std::string data = strs[0];

template<typename Mutex>
void read_thread(Mutex& mutex)
{
    for(int i = 0; i < LOOPS; ++i)
    {
        std::string s;
        {
            std::shared_lock<Mutex> lock(mutex);
            s = data;
        }
        if(s != strs[0] && s != strs[1])
        {
            std::cout << "failed: \"" <<  s << "\"\n";
        }
        assert(s == strs[0] || s == strs[1]);
    }
}

template<typename Mutex>
void write_thread(Mutex& mutex)
{
    for(int i = 0; i < LOOPS; ++i)
    {
        std::unique_lock<Mutex> lock(mutex);
        data = strs[i % 2];
    }
}

template<typename Mutex>
void test_rwlock()
{
    const int cpus = std::thread::hardware_concurrency();
    Mutex mutex;
    std::vector<std::future<void>> futs;
    for(int c = 0; c < cpus; ++c)
    {
        futs.emplace_back(std::async((c == 0)?write_thread<Mutex>:read_thread<Mutex>, std::ref(mutex)));
    }
    for(auto& f: futs)
    {
        f.wait();
    }
}

}

int main(void)
{
    test_rwlock<rwlock>();
    std::cout << "rw tests passed\n";
    test_rwlock<rte_rwlock_wrapper>();
    std::cout << "rte_rwlock_wrapper tests passed\n";
    std::cout << "All tests passed\n";
    return 0;
}
