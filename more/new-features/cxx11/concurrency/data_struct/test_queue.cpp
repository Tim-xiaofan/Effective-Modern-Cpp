#include "queue.h"
#include "queue_with_dummy_node.h"
#include "ring.h"
#include <vector>
#include <iostream>
#include <memory>
#include <cassert>
#include <atomic>

namespace
{

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
				//s_count_.fetch_add(1, std::memory_order_relaxed);
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
				//s_count_.fetch_add(1, std::memory_order_relaxed);
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

		assert(A::count() == 0);
	}
};

void test_ring()
{
	{
		ring<A> q(16);
		assert(q.count() == 0);
		assert(q.empty());
		for(int i = 0; i < 2; ++i)
		{
			A e;
			q.push(0);
			q.push(1);
			assert(q.count() == 2);
			assert(!q.empty());
			{
				auto v = q.to_vector();
				std::vector<A> expected{A(0), A(1)};
				assert(expected == v);
			}

			assert(q.pop(e));
			assert(e == 0);
			q.push(2);
			assert(q.count() == 2);
			assert(!q.empty());

			assert(q.pop(e));
			assert(e == 1);
			assert(q.pop(e));
			assert(e == 2);
			assert(!q.pop(e));
			assert(q.count() == 0);
			assert(q.empty());
		}
	}

	assert(A::count() == 0);
}

}

int main()
{
	test_queue<queue<A>>();
	test_queue<queue_with_dummy_node<A>>();
	test_ring();

	std::cout << "All test passed.\n";
	return 0;
}
