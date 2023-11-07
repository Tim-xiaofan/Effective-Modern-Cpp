#ifndef CXX11_LOCKFREE_TICKETLOCK_H
#define CXX11_LOCKFREE_TICKETLOCK_H
#include <atomic>

class ticketlock
{
    public:
        ticketlock()
            :turn_(0), ticket_(0)
        {
        }

        void lock()
        {
            unsigned me = ticket_.fetch_add(1, std::memory_order_relaxed);
            while(me != turn_.load(std::memory_order_acquire))
            {
            }
        }

        void unlock()
        {
            unsigned new_turn = turn_.load(std::memory_order_relaxed);
            turn_.store(new_turn + 1, std::memory_order_release);
        }

    private:
        std::atomic<unsigned> turn_;
        std::atomic<unsigned> ticket_;
};

#endif
