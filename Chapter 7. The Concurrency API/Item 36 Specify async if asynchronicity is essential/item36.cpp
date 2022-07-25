#include <iostream>
#include <future>
#include <pthread.h>
#include <unistd.h>
#include <thread>

using std::endl;
using std::cout;
using std::cerr;

using namespace std::literals;

int doAsyncWork(int a) noexcept
{
	std::this_thread::sleep_for(1s);
	cout << "child thread-" << pthread_self()<< endl;
	cout << "a=" << a << endl;
	return a;
}

template<typename F, typename... Ts>
inline
#if __cplusplus == 201103
std::future<typename std::result_of<F(Ts...)>::type>
#else
auto
#endif
reallyAsync(F&& f, Ts&&... params)
{
	return std::async(std::launch::async,
				std::forward<F>(f),
				std::forward<Ts>(params)...);
}

int main(int argc, char * argv[])
{
	cout << "parent thread-" << pthread_self()<< endl;

	{
		cout << "\n# std::async: default policy" << endl;
		/**
		  onus of thread mgmt is on implementer of
		  the Standard Library
		 */
		auto fut = std::async(doAsyncWork, 1100);
#ifdef ERROR1 //possible
		/**
		   loop until f has
		   finished running...
		   which may never happen!
		 */
		while(fut.wait_for(100ms) != std::future_status::ready)
		{
		}
#else
		if(fut.wait_for(0ms) == std::future_status::deferred)
		{
		}
		else
		{
			while(fut.wait_for(100ms) != std::future_status::ready)
			{
			}
		}
#endif
		cout << "Done1." << endl;
	}

	{
		cout << "\n# schedule the task for truly asynchronous execution" << endl;
		//auto fut = std::async(std::launch::async, doAsyncWork, 9527);
		auto fut = reallyAsync(doAsyncWork, 9527);
		fut.wait();
		cout << "Done2." << endl;
	}
	return 0;
}
