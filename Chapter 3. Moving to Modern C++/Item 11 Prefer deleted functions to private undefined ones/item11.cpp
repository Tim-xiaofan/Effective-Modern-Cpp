#include <iostream>
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include <string>
#include <atomic>
#include <stdlib.h>
#include <mutex>
#include <list>
#include <type_traits>

using std::endl;
using std::cout;
using std::unique_ptr;
using std::shared_ptr;
using std::vector;
using std::unordered_map;
using std::string;

bool isLucky(int number){ return false;}
bool isLucky(char) = delete; // reject chars
bool isLucky(bool) = delete; // reject bools
bool isLucky(double) = delete; // reject doubles and floats

template<typename T> void processPointer(T* ptr){}
template<> void processPointer<void>(void *) = delete;
template<> void processPointer<char>(char *) = delete;
template<> void processPointer<const char>(const char *) = delete;

class Widget
{
	public:
		template <typename T>
		void processPointer(T* ptr){}
	private:
		//template <> void processPointer<void>(void *); // error
};

template <> 
void Widget::processPointer<void>(void *) = delete;

template<typename T>
class foo
{
	public:
		foo(){}
};

template<>
foo<bool>::foo() = delete;

int main(int argc, char * argv[])
{
	isLucky(0);
	// isLucky(0.0); //error
	// isLucky(true);// error

	processPointer(new int);
	// processPointer(new char); // error
	// processPointer(reinterpret_cast<void *>(new char)); // error
	// processPointer(reinterpret_cast<const char *>(new char)); // error
	processPointer(reinterpret_cast<const void *>(new char)); // fine

	//foo<bool> f; // error
	foo<int> f;
	return 0;
}
