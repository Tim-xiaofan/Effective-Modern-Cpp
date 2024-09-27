#include <iostream>
#include <atomic>
#include <future>
#include <mutex>
#include <vector>
#include <cassert>
#include <shared_mutex>
#include <limits>
#include <cryptopp/integer.h>
#include <algorithm>

namespace
{

class rwlock 
{
    public:
        rwlock() : readers_(0) {}
        
        void lock() 
        {
            int expected = 0;
            // CAS: 保证 0 -> -1 的原子性
            while(!readers_.compare_exchange_weak(expected, 
                            -1, std::memory_order_acquire, 
                            std::memory_order_relaxed)) { // (1)
                expected = 0;
            } // 需要没有任何其他读者和写者，容易饥饿
        }

        void unlock() 
        {
            readers_.store(0, std::memory_order_release); // (2) synchronizes-with (1) or (3)
        }

        void lock_shared() 
        {
            bool success = false;
            do {
                int expected = readers_.load(std::memory_order_relaxed);
                if (expected == -1) { // try again if write lock is held
                    std::this_thread::yield();
                    continue;
                }
                // CAS: 保证 expected -> expected + 1 原子性
                success = readers_.compare_exchange_weak(expected,
                            expected + 1, std::memory_order_acquire,
                            std::memory_order_relaxed); // (3)
            } while(!success); // util read lock is held
        }

        void unlock_shared()
        {
            readers_.fetch_sub(1, std::memory_order_release); // (4) synchronizes-with (1) or (3)
        }
    
    private:
        // -1 write locked, 0 unlocked, > 0 read locked
        std::atomic<int> readers_;
};

CryptoPP::Integer data{std::numeric_limits<int64_t>::max()};
rwlock data_lock;
constexpr int LOOPS = 10000 *100;

std::vector<CryptoPP::Integer> thread_fn(void) 
{
    std::vector<CryptoPP::Integer> ret{};
    for (int i = 0; i < LOOPS; ++i) {
        {
            std::unique_lock lock(data_lock); // (1)
            ++data; // (2) 
        } // unlock: (3) synchronizes-with (1) 

        CryptoPP::Integer tmp;
        {
            std::shared_lock lock(data_lock);
            tmp = data;
        }
        ret.emplace_back(std::move(tmp));
    }
    return ret;
}

template <typename T>
int is_sorted(const std::vector<T>& v) {
    if (v.size() < 2) {
        return 0;  // 对于空或单元素的 vector，视为未排序
    }

    bool ascending = true;
    bool descending = true;

    for (size_t i = 1; i < v.size() && (ascending || descending); ++i) {
        if (v[i] > v[i - 1]) {
            descending = false;  // 不是降序
        } else if (v[i] < v[i - 1]) {
            ascending = false;   // 不是升序
        }
    }

    if (ascending) {
        return 0;  // 升序
    } else if (descending) {
        return 1;  // 降序
    } else {
        return -1; // 无序
    }
}

}

int main(void)
{
    std::vector<std::future<std::vector<CryptoPP::Integer>>> futs;
    int cpus = std::thread::hardware_concurrency();
    assert(cpus > 2);

    CryptoPP::Integer max = CryptoPP::Integer(std::numeric_limits<int64_t>::max()) + cpus * LOOPS;
    CryptoPP::Integer min = CryptoPP::Integer(std::numeric_limits<int64_t>::max());
    std::cout << "[" << min << ", " << max << "]\n";

    for(int i = 0; i < cpus; ++i) {
        futs.emplace_back(std::async(thread_fn));
    }

    for(int i = 0; i < cpus; ++i) {
        auto &fut = futs[i];
        auto v =  fut.get();
        assert(is_sorted(v) == 0); 
        auto last = std::unique(v.begin(), v.end());
        v.erase(last, v.end());
        std::cout << "CPU#" << i << ": " << v.size() << 
            " [" << v[0] << ", " << v.back() << "]\n";
    }

    assert(data == max);

    std::cout << "CPU: " << cpus << ", all done.\n";

    return 0;
}
