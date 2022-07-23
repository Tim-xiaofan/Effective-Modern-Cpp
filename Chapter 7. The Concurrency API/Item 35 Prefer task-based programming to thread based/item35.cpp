#include <iostream>
#include <future>
#include <pthread.h>
#include <unistd.h>

using std::endl;
using std::cout;
using std::cerr;

int doAsyncWork(int a) noexcept
{
	sleep(1);
	cout << "child thread-" << pthread_self()<< endl;
	cout << "a=" << a << endl;
	return a;
}

int main(int argc, char * argv[])
{
	{
		cout << "# std::async" << endl;
		cout << "parent thread-" << pthread_self()<< endl;
		/**
		  onus of thread mgmt is on implementer of
		  the Standard Library
		 */
		auto fut = std::async(doAsyncWork, 1100);
		fut.wait();
		cout << "get:" << fut.get() << endl;
	}
	cout << "Done." << endl;
	return 0;
}
