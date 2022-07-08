#include <iostream>
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include <string>
#include <atomic>
#include <stdlib.h>
#include <mutex>

using std::endl;
using std::cout;
using std::unique_ptr;
using std::shared_ptr;
using std::vector;
using std::unordered_map;
using std::string;

void f(int){ cout << "int" << endl;}
void f(bool){ cout << "bool" << endl;}
void f(void*){ cout << "void*" << endl;}
void * findRecord(void) { return reinterpret_cast<void *>(findRecord);}

class Widget {};
int f1(std::shared_ptr<Widget> spw) { return 0; }
double f2(std::unique_ptr<Widget> upw) { return 0; }
bool f3(Widget *pw) { return false; }

std::mutex f1m, f2m, f3m; // mutexes for f1, f2, and f3
using MuxGuard = std::lock_guard<std::mutex>; // C++11 typedef;

template<typename FuncType, typename MuxType, typename PtrType>
auto lockAndCall(FuncType func, MuxType& mutex, PtrType ptr) -> decltype(func(ptr))
{
	MuxGuard g(mutex);
	return func(ptr);
}

int main(int argc, char * argv[])
{
	{
		cout << "# Avoid overload resolution surprises" << endl;
		f(0); //int
		//f(NULL); //error! ambiguous in int, bool, void *
		f(nullptr); // void*
	}

	{
		cout << "\n# Improve code clarity" << endl;
		auto result = findRecord();
		if(result == nullptr)
		{
			cout << "null result" << endl;
		}
		cout << "result:" << result << endl;
	}

	{
		/** 
		  a mutex wrapper that provides a convenient RAII-style 
		  mechanism for owning a mutex for the duration of a scoped block
		 */
		cout << "\n# fm1" << endl;
		MuxGuard g(f1m);
		auto result = f1(0);
		cout << result << endl;
	}//unlock

	{
		cout << "\n# fm2" << endl;
		MuxGuard g(f2m);
		auto result = f2(NULL);
		cout << result << endl;
	}

	{
		cout << "\n# fm3" << endl;
		MuxGuard g(f3m);
		auto result = f3(nullptr);
		cout << result << endl;
	}

	{
		cout << "\n# All in one" << endl;
		//lockAndCall(f1, f1m, 0); //gcc 9.4.0 error:could not convert ‘ptr’ from 
								 //‘int’ to ‘std::shared_ptr<Widget>’
		//lockAndCall(f2, f2m, NULL); //gcc 9.4.0 error:could not convert ‘ptr’ from 
									//‘long int’ to ‘std::unique_ptr<Widget>’
		lockAndCall(f3, f3m, nullptr);
	}
	return 0;
}
