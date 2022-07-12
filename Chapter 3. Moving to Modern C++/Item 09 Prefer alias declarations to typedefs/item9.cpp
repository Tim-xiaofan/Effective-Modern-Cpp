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

/** same things*/
typedef std::unique_ptr<std::unordered_map<std::string, std::string> > UPtrMapSS;
using UPtrMapSS = std::unique_ptr<std::unordered_map<std::string, std::string> >; 

typedef void (*FP)(int, const std::string &);
using FP = void (*)(int, const std::string &);

class Widget {};

/** alias*/
template<typename T> 
struct MyAlloc 
{
	typedef T value_type;
	void deallocate(T * Tp, int n){}
};

template<typename T>
using MyAllocList = std::list<T, MyAlloc<T> >;
/** typedef*/
template<typename T>
struct MyAllocList1
{
	typedef std::list<T, MyAlloc<T> > type;
};
template <typename T>
class Widget1
{
	private:
		typename MyAllocList1<T>::type list; // dependent type
		MyAllocList<T> list1; // non-dependent type
};

class Wine {};
template<>
class MyAllocList1<Wine>
{
	private:
		enum class WineType {White, Red, Rose};
		WineType type; // type is data member
};

template <typename T>
void type_traits_test(void)
{
	std::remove_const<T>::type; // yields T from const T
	std::remove_reference<T>::type; // yields T from T& and T&&
	std::add_lvalue_reference<T>::type; // yields T& from T
}

int main(int argc, char * argv[])
{
	{
		cout << "# Not compelling reasons" << endl;
	}

	{
		cout << "\n# Compelling reasons: alias templates" << endl;
		MyAllocList<Widget> lw;
		MyAllocList1<Widget>::type lw1;
		//Widget1<Wine> w; // error
	}

	{
		cout << "\n# type traits" << endl;
	}
	return 0;
}
