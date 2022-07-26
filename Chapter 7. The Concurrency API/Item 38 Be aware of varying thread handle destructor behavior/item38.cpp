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

std::vector<std::future<void>> futs;

class Widget
{
	public:
	private:
		std::shared_future<double> fut;
};

int calValue(void)
{
	cout << "thread-" << pthread_self() << endl;
	return 0;
}

int main(int argc, char * argv[])
{
	{
		cout << "# arbitrary future object, it’s not "
			"possible to know whether it will block in its destructor" << endl;
	}

	{
		cout << "\n# std::packaged_task" << endl;
		
		std::packaged_task<int()> pt(calValue);
		auto fut = pt.get_future();
		std::thread t(std::move(pt));
		//t.join();
	}
	return 0;
}
