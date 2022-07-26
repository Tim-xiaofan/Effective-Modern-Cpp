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

std::condition_variable cv; // condvar for event
std::mutex m; // mutex for use with cv
std::atomic<bool> flag{false};

static void
work(void)
{
	usleep(20);
	std::unique_lock<std::mutex> lk(m); // lock mutex
	//cout << "Waiting..." << endl;
	printf("Thread-%ld waiting...\n", pthread_self());
	cv.wait(lk);// wait for notify;
}

static void
work1(void)
{
	while(!flag);
	printf("Thread-%ld exit\n", pthread_self());
}

int main(int argc, char * argv[])
{
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
		sleep(1);
		printf("Thread-%ld set flag...\n", pthread_self());
		flag = true;
	}
	cout << "All done." << endl;
	return 0;
}
