#include <atomic>
#include <cassert>
#include <thread>

class rwlock
{
    public:
        rwlock() : readers_(0) {}
        
        void lock() 
        {
            bool success = false;
            do
            {
                int expected = readers_.load(std::memory_order_relaxed);
                // a lock is held
                if(expected != 0)
                {
                    std::this_thread::yield();
                    continue;
                }
                success = readers_.compare_exchange_weak(
                            expected,
                            -1,
                            std::memory_order_acquire,
                            std::memory_order_relaxed);
            }while(!success);
        }

        void unlock()
        {
            readers_.store(0, std::memory_order_release);
        }

        void lock_shared()
        {
            bool success = false;
            do
            {
                int expected = readers_.load(std::memory_order_relaxed);
                /** write lock is held*/
                if(expected < 0)
                {
                    std::this_thread::yield();
                    continue;
                }
                success = readers_.compare_exchange_weak(
                            expected,
                            expected + 1,
                            std::memory_order_acquire,
                            std::memory_order_relaxed);
            }while(!success);
        }

        void unlock_shared()
        {
            readers_.fetch_sub(1, std::memory_order_release);
        }

    private:
        std::atomic<int> readers_;
};
