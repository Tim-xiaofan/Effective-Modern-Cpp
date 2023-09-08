#include <iostream>
#include <thread>

int main()
{
	// Returns the number of concurrent threads supported by the implementation.
	// The value should be considered only a hint.
	// 在多核系统中，返回逻辑核的数量
	unsigned int n = std::thread::hardware_concurrency(); //
	std::cout << n << " concurrent threads are supported\n";
	return 0;
}
