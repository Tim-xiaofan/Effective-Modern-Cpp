#ifndef CXX11_LOCKFREE_SPINLOCK_H
#define CXX11_LOCKFREE_SPINLOCK_H
#include <atomic>

class spinlock
{
    public:
        spinlock() :flag_(ATOMIC_FLAG_INIT) {}

        void lock()
        {
            while(flag_.test_and_set(std::memory_order_acquire))
            {
            }
        }

        void unlock()
        {
            flag_.clear(std::memory_order_release);
        }

    private:
        std::atomic_flag flag_;
};

#endif
