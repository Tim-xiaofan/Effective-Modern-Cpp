#include <iostream>
#include <memory>
#include <vector>

using std::endl;
using std::cout;
using std::cerr;

#if __cplusplus <= 201103
namespace std
{//N3656 by Stephan T. Lavavej
	template<class T> struct _Unique_if {
        typedef unique_ptr<T> _Single_object;
    };

    template<class T> struct _Unique_if<T[]> {
        typedef unique_ptr<T[]> _Unknown_bound;
    };

    template<class T, size_t N> struct _Unique_if<T[N]> {
        typedef void _Known_bound;
    };

    template<class T, class... Args>
        typename _Unique_if<T>::_Single_object
        make_unique(Args&&... args) {
            return unique_ptr<T>(new T(std::forward<Args>(args)...));
        }

    template<class T>
        typename _Unique_if<T>::_Unknown_bound
        make_unique(size_t n) {
            typedef typename remove_extent<T>::type U;
            return unique_ptr<T>(new U[n]());
        }

    template<class T, class... Args>
        typename _Unique_if<T>::_Known_bound
        make_unique(Args&&...) = delete;
}
#endif

void f(const std::vector<int>& v)
{
	for(const auto& i: v)
	{
		cout << i << " ";
	}
	cout << endl;
}

void f(const void* ptr)
{
	cout << "ptr:" << ptr << endl;
}

#ifndef ERROR2
void f(std::size_t val)
{
	cout << "val:" << val << endl;
}
#endif

void f1(int (*pf)(int)) // pf = "processing function"
{}


int processVal(int value) { return value; }
int processVal(int value, int priority) { return value;}

template<typename T>
T workOnVal(T param) // template for processing values
{return param;}

template<typename T>
void fwd(T&& param) // accept any argument
{
	f(std::forward<T>(param)); // forward it to f
}

template<typename T>
void fwd1(T&& param)
{
	f1(std::forward<T>(param));
}

class Widget 
{
	public:
		static const std::size_t MinVals = 28; // MinVals' declaration
};

#ifndef ERROR3
const std::size_t Widget::MinVals;
#endif

struct IPv4Header 
{
	std::uint32_t version:4,
		IHL:4,
		DSCP:6,
		ECN:2,
		totalLength:16;
	IPv4Header():version(0), IHL(0), DSCP(0), ECN(0),totalLength(0){}
};

int main(int argc, char * argv[])
{
	{
		cout << "# Braced initializers" << endl;
		f({1, 2, 3});

#ifdef ERROR1
		fwd({1, 2, 3}); // error! doesn't compile
#else
		auto il = {1, 2, 3};
		fwd(il);
#endif
	}

	{
		cout << "\n# 0 or NULL as null pointers" << endl;
#ifdef ERROR2
		fwd(NULL); //error! doesn't compile
		fwd(0); //ditto
#else
		fwd(nullptr);
#endif
	}

	{
		cout << "\n# Declaration-only integral static const data members" << endl;
		std::vector<int> widgetData;
		widgetData.reserve(Widget::MinVals); // use of MinVals

		f(Widget::MinVals); //fine, treat as f(28)
#ifdef ERROR3
		fwd(Widget::MinVals); //error, shouldn't link
#else
		fwd(Widget::MinVals);
#endif
	}

	{
		cout << "\n# Overloaded function names and template names" << endl;
		f1(processVal); // fine
#ifdef ERROR4
		fwd1(processVal); // error! which processVal?
		fwd1(workOnVal);
#else
		using ProcessFuncType = int (*)(int);
		ProcessFuncType pf = processVal;
		fwd1(pf);
		fwd1(static_cast<ProcessFuncType>(workOnVal));
#endif
	}

	{
		cout << "\n# Bitfields" << endl;
		IPv4Header h;
		f(h.totalLength); //fine
		//cout << "&h.totalLength" << &h.totalLength << endl; // error
#ifdef ERROR5
		fwd(h.totalLength); //error!
#else
		// copy bitfield value; see Item 6 for info on init. form
		auto length = static_cast<std::uint16_t>(h.totalLength);
		fwd(length);
#endif
	}
	return 0;
}
