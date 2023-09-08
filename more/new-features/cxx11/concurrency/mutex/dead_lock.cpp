#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

namespace
{

using mutex = std::timed_mutex;
mutex mutexa, mutexb;

void thread1()
{
	std::cout << "thread-1 acquiring mutexa\n";
	std::lock_guard<mutex> locka(mutexa);
	std::cout << "thread-1 acquired mutexa\n";

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	
	std::cout << "thread-1 acquiring mutexb\n";
	std::lock_guard<mutex> lockb(mutexb);
	std::cout << "thread-1 acquired mutexb\n";

	std::cout << "thread-1 finished\n";
}

void thread2()
{
	std::cout << "thread-2 acquiring mutexb\n";
	std::lock_guard<mutex> lockb(mutexb);
	std::cout << "thread-2 acquired mutexb\n";
	
	std::this_thread::sleep_for(std::chrono::milliseconds(25));
	
	std::cout << "thread-2 acquiring mutexa\n";
	std::lock_guard<mutex> locka(mutexa);
	std::cout << "thread-2 acquired mutexa\n";
	
	std::cout << "thread-2 finished\n";
}

void thread3()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	std::cout << "thread-3 acquiring mutexa\n";
	std::unique_lock<mutex> locka(mutexa, std::chrono::seconds(30));
	// tests whether the lock owns (i.e., has locked) its associated mutex
	if(locka)
	{
		std::cout << "thread-3 acquired mutexa\n";
		
		std::cout << "thread-3 acquiring mutexb\n";
		std::unique_lock<mutex> lockb(mutexb, std::chrono::seconds(30));
		// tests whether the lock owns (i.e., has locked) its associated mutex
		if(lockb.owns_lock())
		{
			std::cout << "thread-3 acquires mutexb failed\n";
		}
	}
	else
	{
		std::cout << "thread-3 acquires mutexa failed\n";
		return;
	}
	std::cout << "thread-3 finished\n";
}

}

int main()
{
	{
		std::thread t1(thread1), t2(thread2), t3(thread3);
		t1.join();
		t2.join();
		t3.join();
	}
	return 0;
}
