#include <iostream>
#include <string>
#include <thread>
#include <chrono>

namespace
{

void thread1()
{
	std::cout << '[' << __PRETTY_FUNCTION__ << "] thread-" << std::this_thread::get_id() << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 线程休眠某个指定的时间片，才被重新唤醒；
}

void thread2(std::string& str)
{
	std::cout << '[' << __PRETTY_FUNCTION__ << "] thread-" << std::this_thread::get_id() << std::endl;
	str = "changed";
	std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::microseconds(100)); // 线程休眠至某个指定的时刻，才被重新唤醒
}

}

int main()
{
	std::string str = "main";
	std::thread t1(thread1);
	std::thread t2(thread2, std::ref(str));
	t1.join();
	std::cout << "thread-" << t1.get_id() << " has been joined\n";
	t2.join();
	std::cout << "thread-" << t2.get_id() << " has been joined\n";
	std::cout << str << std::endl;
	return 0;
}
