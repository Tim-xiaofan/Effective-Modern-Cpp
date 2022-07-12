#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <stdlib.h>
#include <list>
#include <algorithm>
#include <array>
#include <mutex>
#include <cmath>
#include <atomic>
#include <cstdlib>

#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

using std::endl;
using std::cout;
using std::vector;
using std::string;

class Polynominal
{
	public:
		using RootsType = 
			std::vector<double>;
		RootsType roots() const
		{// keep thread-safe
			std::lock_guard<std::mutex> g(m); //lock mutex
			if(!rootsAreValid)
			{ // if cache not valid compute/store roots
				rootsAreValid = true;
			}
			return rootVals;
		}

	private:
		mutable std::mutex m; //unlock mutex
		mutable bool rootsAreValid{false};
		mutable RootsType rootVals{};
};

class Point
{//less expensive
	public:
		double distanceFromOrigin() const noexcept
		{// keep thread-safe
			++callCount;
			return std::sqrt((x * x) + (y * y));
		}
	private:
		mutable std::atomic<unsigned> callCount{0};
		double x{0}, y{0};
};

pthread_t expensiveComputation1(void)
{
	cout << "computing..." << endl;
	for(int i = 0; i < 10000 * 10000; ++i)
	{
	}
	return pthread_self();
}

#define  expensiveComputation2() expensiveComputation1()

class Widget
{
	public:
		pthread_t magicValue() const
		{
			if(cacheValid) return cachedValue;
			else
			{
				auto val1 = expensiveComputation1();
				auto val2 = expensiveComputation2();
				cachedValue = val1 + val2;
				cacheValid = true;
				return cachedValue;
			}
		}
	private:
		mutable std::atomic<bool> cacheValid{ false };
		mutable std::atomic<pthread_t> cachedValue;
};

static pthread_t val = 0;

void * work(void * arg)
{
	Widget * wp = reinterpret_cast<Widget *>(arg);
	
	if(wp == nullptr) return nullptr;
	cout << "thread-"<< pthread_self() <<": " <<  (val = wp->magicValue()) << endl;

	return nullptr;
}

int main(int argc, char * argv[])
{
	cout << "problem of atomic" << endl;

	pthread_t tid, val1;
	Widget w;
	if(pthread_create(&tid, NULL, work, (void *)&w) != 0)
	{
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	cout << "thread-"<< pthread_self() <<": " << (val1 = w.magicValue()) << endl;
	pthread_join(tid, nullptr);

	if(val != val1)
	{
		std::cerr << "std::atomic does not work in this case, use mutex" << endl;
	}
	return 0;
}
