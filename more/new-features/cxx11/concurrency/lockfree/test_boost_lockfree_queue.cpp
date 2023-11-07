#include <iostream>
#include <sstream>
#include <iomanip>
#include <future>
#include <vector>
#include <mutex>
#include <random>
#include <chrono>
#include <thread>
#include <cassert>
#include <atomic>
#include <chrono>
#include <functional>
#include <string>
#include <cstring>
#include <algorithm>
#include <boost/lockfree/queue.hpp>

namespace
{

constexpr unsigned MB = 1024 * 1024;
constexpr unsigned MAX_LOOP = 1000 * 10000;

std::atomic_uint push_count{0};
std::atomic_uint pop_count{0};
std::chrono::steady_clock::time_point start;
std::chrono::steady_clock::time_point end;
std::atomic_int customers;
std::atomic_flag first_producer;

template<typename Queue>
void customer(int id, Queue& q)
{
	pthread_setname_np(pthread_self(), "customer");
	typename Queue::value_type i;
	while(true)
	{
		if(q.pop(i))
		{
			if(pop_count.fetch_add(1, std::memory_order_relaxed) + 1 < MAX_LOOP)
			{
				continue;
			}
			else
			{
				break;
			}
		}
	}
	// last customer
	if(customers.fetch_sub(1, std::memory_order_relaxed) == 1)
	{
		end = std::chrono::steady_clock::now();
	}

}

template<typename Queue>
void producer(int id, Queue& q)
{
	pthread_setname_np(pthread_self(), "producer");
	if(first_producer.test_and_set(std::memory_order_relaxed) == false)
	{// fisrt producer
		start = std::chrono::steady_clock::now();
	}
	while(true)
	{
		auto i = push_count.fetch_add(1, std::memory_order_relaxed);
		while(!q.push(i));
		if(i + 1 < MAX_LOOP)
		{
			continue;
		}
		else
		{
			break;
		}
	}
}

template<typename Queue>
void work_thread(int id, Queue& q)
{
	(id % 2 == 0)? customer(id, q): producer(id, q);
}

template<typename Queue, typename Work>
void test_queue(Queue& q, Work work_thread, size_t total_size)
{
	const int cpus = std::thread::hardware_concurrency();
	assert(cpus > 1);
    for(int c = 2; c <= cpus; c+= 2)
    {
        customers.store(c/2, std::memory_order_relaxed);
        std::vector<std::future<void>> futs;
        for(int i = 0; i < c; ++i)
        {
            auto fut = std::async(work_thread, i, std::ref(q));
            futs.emplace_back(std::move(fut));
        }
        for(auto& f: futs)
        {
            f.wait();
        }
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        auto s = std::chrono::duration_cast<std::chrono::seconds>(us);
        std::cout << "c=" << c << ", ";
        std::cout << total_size * 1.0 / MB / s.count() << "MB/s\n";
        pop_count.store(0, std::memory_order_relaxed);
        push_count.store(0, std::memory_order_relaxed);
        first_producer.clear(std::memory_order_relaxed);
    }
}

} // namespace

int main()
{
    size_t total_size = sizeof(long long unsigned) * MAX_LOOP;
    boost::lockfree::queue<long long unsigned, boost::lockfree::capacity<1024>> q;
    test_queue(q, work_thread<decltype(q)>, total_size);
    return 0;
}
