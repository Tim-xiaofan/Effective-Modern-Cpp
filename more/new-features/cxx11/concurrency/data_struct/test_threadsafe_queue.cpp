#include "threadsafe_single_list_queue.h"
#include "threadsafe_std_queue.h"
#include "threadsafe_queue_with_dummy_node.h"
#include "threadsafe_ring.h"
#include "threadsafe_sys_stailq.h"
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
#include <algorithm>
#include <boost/lockfree/queue.hpp>

//#define DEBUG 1

namespace
{

#ifdef DEBUG
std::mutex iomutex;
thread_local std::random_device seed{};
thread_local std::mt19937 generator{seed()};
thread_local std::uniform_int_distribution<int> distributor(10, 20);
#endif

std::atomic_uint push_count{0};
std::atomic_uint pop_count{0};
std::chrono::steady_clock::time_point start;
std::chrono::steady_clock::time_point end;
std::atomic_int customers;
std::atomic_flag first_producer;
constexpr unsigned MAX_LOOP = 1000 * 10000;
constexpr unsigned CAPACITY = 1024;

template<typename Q>
void customer(int id, Q& q)
{
	pthread_setname_np(pthread_self(), "customer");
	while(true)
	{
		auto ptr = q.try_pop();
		if(ptr)
		{
#ifdef DEBUG
			{
				std::lock_guard<std::mutex> lock(iomutex);
				std::cout << "customer-" << id << " pop:" << *ptr << std::endl;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(distributor(generator)));
#endif
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

template<typename Q>
void customer_adapter(int id, Q& q)
{
	pthread_setname_np(pthread_self(), "customer");
	typename Q::value_type i;
	while(true)
	{
		if(q.pop(i))
		{
#ifdef DEBUG
			{
				std::lock_guard<std::mutex> lock(iomutex);
				std::cout << "customer-" << id << " pop:" << i << std::endl;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(distributor(generator)));
#endif
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

template<typename T>
void customer(int id, boost::lockfree::queue<T, boost::lockfree::capacity<CAPACITY>>& q)
{
	customer_adapter(id, q);
}

template<typename T>
void customer(int id, threadsafe_ring<T>& q)
{
	customer_adapter(id, q);
}

template<typename Q>
void producer(int id, Q& q)
{
	pthread_setname_np(pthread_self(), "producer");
	if(first_producer.test_and_set(std::memory_order_relaxed) == false)
	{// fisrt producer
		start = std::chrono::steady_clock::now();
	}
	while(true)
	{
		auto i = push_count.fetch_add(1, std::memory_order_relaxed);
		q.push(i);
#ifdef DEBUG
		{
			std::lock_guard<std::mutex> lock(iomutex);
			std::cout << "producer-" << id << " push: " << i << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(distributor(generator)));
#endif
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

template<typename Q>
void producer_adapter(int id, Q& q)
{
	pthread_setname_np(pthread_self(), "producer");
	if(first_producer.test_and_set(std::memory_order_relaxed) == false)
	{// fisrt producer
		start = std::chrono::steady_clock::now();
	}
	while(true)
	{
		auto i = push_count.fetch_add(1, std::memory_order_relaxed);
		while(!q.push(i)) {}
#ifdef DEBUG
		{
			std::lock_guard<std::mutex> lock(iomutex);
			std::cout << "producer-" << id << " push: " << i << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(distributor(generator)));
#endif
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

template<typename T>
void producer(int id, boost::lockfree::queue<T, boost::lockfree::capacity<CAPACITY>>& q)
{
	producer_adapter(id, q);
}

template<typename T>
void producer(int id, threadsafe_ring<T>& q)
{
	producer_adapter(id, q);
}

template<typename Q>
void work_thread(int id, Q& q)
{
	(id % 2 == 0)? customer(id, q): producer(id, q);
}

class A
{
	public:
		A(int v = 0) : value_(v)
		{
			s_count_.fetch_add(1, std::memory_order_relaxed);
		}
		
		~A()
		{
			s_count_.fetch_sub(1, std::memory_order_relaxed);
		}

		A(const A& rhs)
		{
			value_ = rhs.value_;
			s_count_.fetch_add(1, std::memory_order_relaxed);
		}
		
		A& operator=(const A& rhs)
		{
			if(&rhs != this)
			{
				value_ = rhs.value_;
				s_count_.fetch_add(1, std::memory_order_relaxed);
			}
			return *this;
		}

		A(A&& rhs)
		{
			value_ = rhs.value_;
			s_count_.fetch_add(1, std::memory_order_relaxed);
		}
		
		A& operator=(A&& rhs)
		{
			if(&rhs != this)
			{
				value_ = rhs.value_;
				s_count_.fetch_add(1, std::memory_order_relaxed);
			}
			return *this;
		}

		operator int() const
		{
			return value_;
		}

		static int count()
		{
			return s_count_.load(std::memory_order_relaxed);
		}

	private:
		int value_;
		static std::atomic_int s_count_;
};

std::atomic_int A::s_count_{0};

template <typename Q>
struct test_queue
{
	void operator()()
	{
		{
			Q q;
			using A = typename Q::value_type;

			assert(q.count() == 0);
			assert(q.empty());
			for(int i = 0; i < 2; ++i)
			{
				q.push(0);
				q.push(1);
				assert(q.count() == 2);
				assert(!q.empty());
				{
					auto v = q.to_vector();
					std::vector<A> expected{A(0), A(1)};
					assert(expected == v);
				}

				assert(*q.try_pop() == 0);
				q.push(2);
				assert(q.count() == 2);
				assert(!q.empty());

				assert(*q.try_pop() == 1);
				assert(*q.try_pop() == 2);
				assert(!q.try_pop());
				assert(q.count() == 0);
				assert(q.empty());
			}
			q.push(0);
			q.push(1);
		}
		assert(A::count() == 0);
	}
};

constexpr int N = 6;
using func_t = std::function<void (int)>;

} // namespace

int main()
{
	const int cpus = std::thread::hardware_concurrency();
	assert(cpus > 1);

	test_queue<threadsafe_single_list_queue<A>>();
	test_queue<threadsafe_std_queue<A>>();
	test_queue<threadsafe_queue_with_dummy_node<A>>();
	test_queue<threadsafe_sys_stailq<A>>();

	threadsafe_std_queue<unsigned> q;
	threadsafe_single_list_queue<unsigned> q1;
	threadsafe_queue_with_dummy_node<unsigned> q2;
	boost::lockfree::queue<unsigned, boost::lockfree::capacity<CAPACITY>> q3;
	threadsafe_ring<unsigned> q4(CAPACITY);
	threadsafe_sys_stailq<unsigned> q5;
	constexpr size_t total_size = MAX_LOOP * sizeof(unsigned);

	func_t funcs[N];
	funcs[0] = [&q](int id) { work_thread(id, q); };
	funcs[1] = [&q1](int id) { work_thread(id, q1); };
	funcs[2] = [&q2](int id) { work_thread(id, q2); };
	funcs[3] = [&q3](int id) { work_thread(id, q3); };
	funcs[4] = [&q4](int id) { work_thread(id, q4); };
	funcs[5] = [&q5](int id) { work_thread(id, q5); };
	std::string names[N] = {
		"threadsafe_std_queue",
		"threadsafe_single_list_queue",
		"threadsafe_queue_with_dummy_node",
		"boost::lockfree::queue",
		"threadsafe_ring",
		"threadsafe_sys_stailq",
	};

	std::cout << "MAX_LOOP: " << MAX_LOOP << std::endl;
	size_t width = 0;
	for(int i = 0; i < N; ++i)
	{
		width = std::max(width, names[i].size()) + 2;
	}
	std::cout << "\t";
	for(int i = 0; i < N; ++i)
	{
		std::cout << std::left << std::setw(width) << names[i];
	}
	std::cout << std::endl;
	for(int c = 2; c <= cpus; c+= 2)
	{
		std::cout << "c=" << c << "\t";
		std::flush(std::cout);
		for(int n = 0; n < N; ++n)
		{
			customers.store(c/2, std::memory_order_relaxed);
			std::vector<std::future<void>> futs;
			for(int i = 0; i < c; ++i)
			{
				futs.emplace_back(std::async(funcs[n], i));
			}
			for(auto& f: futs)
			{
				f.wait();
			}
			auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(us);
			std::stringstream oss;
			oss << ms.count() << "ms(" << total_size / ms.count() << "B/ms)";
			std::cout << std::left << std::setw(width) << oss.str();
			std::flush(std::cout);
			pop_count.store(0, std::memory_order_relaxed);
			push_count.store(0, std::memory_order_relaxed);
			first_producer.clear(std::memory_order_relaxed);
		}
		std::cout << std::endl;
	}
	return 0;
}
