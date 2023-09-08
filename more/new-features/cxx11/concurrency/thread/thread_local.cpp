#include <iostream>
#include <thread>
#include <string>
#include <mutex>

namespace
{
thread_local int range = 1;
std::mutex cout_mutex;

void increase_range(const std::string& name)
{
	++range;
	std::lock_guard<std::mutex> lock(cout_mutex);
	std::cout << "thread-" << name << ": range=" << range << std::endl;
}

}

int main()
{
	std::thread a(increase_range, "a"), b(increase_range, "b");
	{
		std::lock_guard<std::mutex> lock(cout_mutex);
		std::cout << "thread-main" << ": range=" << range << std::endl;
	}
	a.join();
	b.join();
	return 0;
}
