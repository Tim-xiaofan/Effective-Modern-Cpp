#include <iostream>
#include <future>
#include <pthread.h>
#include <unistd.h>
#include <thread>
#include <functional>
#include <vector>
#include <cstdlib>
#include <stdio.h>
#include <atomic>

using std::endl;
using std::cout;
using std::cerr;

class ThreadRAII
{
	public:
		enum class DtorAction{join, detach};

		ThreadRAII(std::thread&& t, DtorAction a)
			:action(a), t(std::move(t)){}

		ThreadRAII(const ThreadRAII&) = default;
		ThreadRAII(ThreadRAII&&) = default;

		~ThreadRAII(void)
		{
			if (t.joinable())
			{
				if(action == DtorAction::join)
				{
					t.join();
				}
				else
				{
					t.detach();
				}
			}
		}

		std::thread& get(void) { return t;}

	private:
		DtorAction action;
		std::thread t;
};

static std::atomic<int> ai{0};

static void work(void)
{
	cout << "ai:" << ai << endl;
}

static std::atomic<int> ac{0};
static volatile int vc = 0;
static void work1(void)
{
	++ac;
	++vc;
}

int main(int argc, char *argv[])
{
	{
		cout << "# std::atomic" << endl;
		auto t = ThreadRAII(std::thread(work), ThreadRAII::DtorAction::join);
		ai = 10; // atomically set ai to 10
		std::cout << ai << endl;; // atomically read ai's value
		++ai;
		--ai;
	}

	{
		cout << "\n# std::atomic vs volatile" << endl;
		auto t = ThreadRAII(std::thread(work1), ThreadRAII::DtorAction::join);
		++ac;
		++vc;
	}
	cout << "ac:" << ac << endl;
	cout << "vc:" << vc << endl;
	cout << "All done." << endl;

	{
		cout << "\n# volatile is the way we tell compilers "
			"that we’re dealing with special memory" << endl;
		volatile int x; //read x
		auto y = x; // read x again (can't be optimized away)

		cout << std::boolalpha << std::is_same<volatile int, decltype(y)>::value << endl;
		cout << std::boolalpha << std::is_same<int, decltype(y)>::value << endl;

		x = 10; // write x (can't be optimized away)
		x = 20; // write x again
	}

	{
		cout << "\n# std::atomic" << endl;
		std::atomic<int> x{110};
		std::atomic<int> y(x.load());
		y.store(x.load());
		cout << x << endl;
		cout << y << endl;
	}

	{
		cout << "\n# a memory-mapped I/O location that was "
			"concurrently accessed by multiple threads" << endl;
		volatile std::atomic<int> vai{111};
		cout << vai << endl;
	}
	return 0;
}
