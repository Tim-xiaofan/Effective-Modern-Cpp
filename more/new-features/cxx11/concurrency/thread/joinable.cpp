#include <iostream>
#include <thread>
#include <chrono>
 
void foo()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
 
int main()
{
    std::cout << std::boolalpha;
 
    std::thread t; // default constructed thread is not joinable
    std::cout << "before starting, joinable: " << t.joinable() << '\n';
 
    t = std::thread{foo};
    std::cout << "after starting, joinable: " << t.joinable() << '\n';
 
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    t.join(); // A thread that has finished executing code, but has not yet been joined is still considered an active thread of execution and is therefore joinable
    std::cout << "after joining, joinable: " << t.joinable() << '\n';
 
    t = std::thread{foo};
    t.detach();
    std::cout << "after detaching, joinable: " << t.joinable() << '\n';
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
	return 0;
}
