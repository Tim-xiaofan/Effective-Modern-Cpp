#include <atomic>
#include <iostream>
#include <vector>
#include <type_traits>
#include <cassert>

namespace
{

struct A
{
	int a;
	int b;
};

struct B
{
	int a[48];
};

struct B1
{
	int a[2];
};

struct B2
{
	int a[3];
};

class C
{
	public:
		virtual void fly();
};

class D: public C
{
};

static_assert(std::is_trivially_copyable<A>::value);
static_assert(std::is_trivially_copyable<B>::value);
static_assert(!std::is_trivially_copyable<C>::value);
static_assert(!std::is_trivially_copyable<D>::value);

} //<end namespace>

int main()
{
	assert(std::atomic<A>{}.is_lock_free());
	assert(!std::atomic<B>{}.is_lock_free());
	assert(std::atomic<B1>{}.is_lock_free());
	assert(!std::atomic<B2>{}.is_lock_free());
	std::cout << "All test passed.\n";
	return 0;
};
