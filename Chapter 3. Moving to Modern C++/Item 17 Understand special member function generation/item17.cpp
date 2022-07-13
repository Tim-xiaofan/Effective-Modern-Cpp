#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <stdlib.h>
#include <list>
#include <algorithm>
#include <array>
#include <mutex>
#include <cmath>
#include <atomic>
#include <cstdlib>

using std::endl;
using std::cout;
using std::vector;
using std::string;

template<typename T, typename ...Args>
void drop(T & t, Args&... rest)
{
}

int main(int argc, char * argv[])
{
	{
		class Widget
		{
			public:
				Widget(void) = default;
				Widget(const Widget & w) =default;
		};
		cout << "# The two copy operations are independent" << endl;

		Widget w1, w2;
		w1 = w2; // fine, use default copy assignment
	}

	{
		cout << "\n# The two move operations are not independent" << endl;
		class Widget
		{
			public:
				Widget(void) = default;
				Widget(Widget && w) = default; // cause copy assignment to be deleted
		};
		Widget w1, w2;
		//w1 = std::move(w2);// error! move assignment is deleted
		drop(w1, w2); // preclude unsed warning
	}

	{
		cout << "\n# move operations won’t be generated for any class "
			"that explicitlydeclares a copy operation" << endl;
		class Widget
		{
			public:
				Widget(void) = default;
				Widget(const Widget & w)=default;
				Widget & operator=(const Widget & w) 
				{ cout << "copy assignment" << endl; return *this;}
		};
		Widget w1, w2;
		w2 = std::move(w1); // can compile and run, but use copy assignment
	}

	{
//#define FIX 1
		cout << "declaring a destructor prevents the move operations "
			"from being generated." << endl;
		class Widget
		{
			public:
				Widget(void)
				{ 
					_a = new char[8];
					cout << "alloc:" <<reinterpret_cast<void *>(_a) << endl;
				}
#ifdef FIX
				Widget& operator=(Widget&& w)
				{
					if(this != &w)
					{
						delete _a;	
						_a = w._a;
						w._a = nullptr;
					}
					return *this;
				}
#endif
				~Widget() 
				{
					cout << "free:" <<reinterpret_cast<void *>(_a) << endl;
					delete _a;
				}
			public:
				char *_a;
		};
		Widget w1, w2;
		w2 = std::move(w1); // can compile, but use copy assignment and double free
		cout << "w1._a:" << reinterpret_cast<void*>(w1._a) << endl;
		cout << "w2._a:" << reinterpret_cast<void*>(w2._a) << endl;
	}// 
	return 0;
}
