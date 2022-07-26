#include <iostream>
#include <future>
#include <pthread.h>
#include <unistd.h>
#include <thread>
#include <functional>
#include <vector>
#include <cstdlib>

using std::endl;
using std::cout;
using std::cerr;

static constexpr auto tenMillion = 10000000;
#ifdef ERROR1
static bool
doWork(std::function<bool (int)> filter, int maxVal = tenMillion)
{
	std::vector<int> goodVals;
	std::thread t(
				[&filter, maxVal, &goodVals]
				{
					for(auto i = 0; i < tenMillion; ++i)
						if(filter(i)) goodVals.push_back(i);
				}
				);
	//auto nh = t.native_handle();
	if(std::rand() % 2)
	{
		t.join();
		cout << "size:" << goodVals.size() << endl;
		return true;
	}
	return false; //throw an exception:terminate called without an active exception
}
#endif

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

static bool
doWork1(std::function<bool (int)> filter, int maxVal = tenMillion)
{
	std::vector<int> goodVals;
	ThreadRAII t(
				std::thread(
					[&filter, maxVal, &goodVals]
					{
						for(auto i = 0; i < tenMillion; ++i)
							if(filter(i)) goodVals.push_back(i);
					}
					), 
				ThreadRAII::DtorAction::join);
	//auto nh = t.get().native_handle();
	if(std::rand() % 3 != 0)
	{
		t.get().join();
		cout << "size:" << goodVals.size() << endl;
		return true;
	}
	return false; //throw an exception:terminate called without an active exception
}


int main(int argc, char * argv[])
{
	std::srand(time(NULL));

	{
		cout << "# Thread’s problems: not join and not detach" << endl;
#ifdef ERROR1
		doWork([](int x) {return (x % 9527 == 0);});
#endif
	}

	{
		cout << "\n# Thread RAII:  join could lead to performance anomalies" << endl;
		doWork1([](int x) {return (x % 9527 == 0);});
	}
	return 0;
}
