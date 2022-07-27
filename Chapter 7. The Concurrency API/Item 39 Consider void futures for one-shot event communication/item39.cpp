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

static std::condition_variable cv; // condvar for event
static std::mutex m; // mutex for use with cv
static std::atomic<bool> flag{false};
static bool flag1(false);
static std::promise<void> p;
static std::promise<void> p1;
static std::promise<void> p2;

static void
work(void)
{
	usleep(20);
	std::unique_lock<std::mutex> lk(m); // lock mutex
	printf("Thread-%ld waiting...\n", pthread_self());
	cv.wait(lk);// wait for notify;
}

static void
work1(void)
{
	while(!flag);
	printf("Thread-%ld exit\n", pthread_self());
}

static void
work2(void)
{
	std::unique_lock<std::mutex> lk(m); // as before

	cv.wait(lk, [](){return flag1;}); //  use lambda to avoid

	printf("Thread-%ld exit\n", pthread_self());
}

static void
work3(void)
{
	p.get_future().wait();
	printf("Thread-%ld exit\n", pthread_self());
}

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

static void 
react(void) // func for reacting task
{
	printf("%s:%ld\n", __func__, pthread_self());
}

static void 
detect(void) // func for detecting task
{
	ThreadRAII t(
				std::thread([](){ p1.get_future().wait(); react();}),
				ThreadRAII::DtorAction::join // risky
				);
	// ... 
	// detect hangs if
	// this "…" code throws!
	usleep(0.5 * 1000000); //suspended
	p1.set_value();//unsuspend
}

static void
detect1(void)
{
	auto sf = p2.get_future().share();
	std::vector<ThreadRAII> vt;

	for(int i = 0; i < 5; ++i)
	{
		vt.emplace_back(
					ThreadRAII(
						std::thread([sf](){ sf.wait(); react();}),
						ThreadRAII::DtorAction::join // risky
						)
					);
	}
	
	usleep(0.5 * 1000000); //suspended
	p2.set_value();//unsuspend
}

int main(int argc, char * argv[])
{
	cout << "sizeof(pthread_t):" << sizeof(pthread_t) << endl;

	{// detect event
		cout << "# code smell: wait forever" << endl;
		auto fut = std::async(std::launch::async, work);
#ifndef ERROR1
		usleep(2000);
#endif
		printf("Thread-%ld notify...\n", pthread_self());
		cv.notify_one(); // tell reacting task
	}

	{
		cout << "\n# flag" << endl;
		auto fut = std::async(std::launch::async, work1);
		printf("Thread-%ld set flag...\n", pthread_self());
		flag = true;
	}

	{
		cout << "\n# combine the condvar and flag-based designs" << endl;
		auto fut = std::async(std::launch::async, work2);

		usleep(500); //Don't worry.
		std::lock_guard<std::mutex> g(m);

		flag1 = true;
		cv.notify_one();
	}

	{
		cout << "\n# having the reacting task wait on a future that’s set by the detecting task" << endl;
		auto fut = std::async(std::launch::async, work3);
		usleep(500000); //Don't worry.
		p.set_value();
	}

	{
		cout << "\n# one-shot mechanism" << endl;
		detect();
	}

	{
		cout << "\n# multi reacting tasks" << endl;
		detect1();
	}
	cout << "All done." << endl;
	return 0;
}
