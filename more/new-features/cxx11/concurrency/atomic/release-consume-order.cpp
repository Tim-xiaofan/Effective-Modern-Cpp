// C++11 memory-order
#include <iostream>
#include <atomic>
#include <future>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <cassert>
#include <memory>
#include <type_traits>
#include <string>

namespace
{

std::atomic<std::string*> ptr;
int data;
 
void producer()
{
    std::string* p = new std::string("Hello");
    data = 42;
    ptr.store(p, std::memory_order_release);
}

// Carries dependency
// Within the same thread, evaluation A that is sequenced-before evaluation B may also carry a dependency into B (that is, B depends on A), if any of the following is true:
// 
// 1) The value of A is used as an operand of B, except
//	a) if B is a call to std::kill_dependency,
//	b) if A is the left operand of the built-in &&, ||, ?:, or , operators.
// 2) A writes to a scalar object M, B reads from M.
// 3) A carries dependency into another evaluation X, and X carries dependency into B.
 
void consumer()
{
    std::string* p2;
    while (!(p2 = ptr.load(std::memory_order_consume)))
        ;
    assert(*p2 == "Hello"); // never fires: *p2 carries dependency from ptr
    assert(*p2 + " World" == "Hello World"); // never fires: carries dependency since 3)
    assert(data == 42); // may or may not fire: data does not carry dependency from ptr
}
 
} // namespace

int main(void)
{
	{
		std::thread t1(producer);
		std::thread t2(consumer);
		t1.join(); t2.join();
	}
	std::cout << "All done.\n";
	return 0;
}
