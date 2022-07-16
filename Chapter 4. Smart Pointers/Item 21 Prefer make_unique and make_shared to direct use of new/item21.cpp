#include <iostream>
#include <memory>
#include <atomic>
#include <vector>
#include <string>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <string>
#include <exception>

using std::endl;
using std::cout;
using std::cerr;
using std::string;

#if __cplusplus == 201103
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
			//cout << "make_unique is called" << endl;
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

class Widget 
{
	public:
		Widget(){ cout << "Widget::construct @" << this << endl;}
		~Widget(){ cout << "Widget::destruct @" << this << endl;}
};

int computePriority(void)
{
	cout << "computePriority is called" << endl;
	throw "computePriority exception";
	return 0;
}

void processWidget(int priority, std::shared_ptr<Widget> spw)
{//gcc version 11.2.0
}

void processWidget1(std::shared_ptr<Widget> spw, int priority)
{//gcc version 11.2.0
}

template <typename C>
void dump(const C &c)
{
	for(const auto & i: c)
	  cout << i << " ";
	cout << endl;
}

class ReallyBigType
{
	public:
		ReallyBigType(){ cout << "ReallyBigType construct @" << this << endl;}
		~ReallyBigType(){ cout << "ReallyBigType destruct @" << this << endl;}
	private:
		char _data[1024 * 1024 * 64]; //64 MB
};

int main(int argc, char * argv[])
{
#if __cplusplus == 201103
	{
		using std::make_unique;
		cout << "#c++11 make_unique test" << endl;
		cout << *make_unique<int>() << endl;
		cout << *make_unique<int>(1729) << endl;
		cout << "\"" << *make_unique<string>() << "\"" << endl;
		cout << "\"" << *make_unique<string>("meow") << "\"" << endl;
		cout << "\"" << *make_unique<string>(6, 'z') << "\"" << endl;

		auto up = make_unique<int[]>(5);

		for (int i = 0; i < 5; ++i) {
			cout << up[i] << " ";
		}

		cout << endl;

#if defined(ERROR1)
		auto up1 = make_unique<string[]>("error");
#elif defined(ERROR2)
		auto up2 = make_unique<int[]>(10, 20, 30, 40);
#elif defined(ERROR3)
		auto up3 = make_unique<int[5]>();
#elif defined(ERROR4)
		auto up4 = make_unique<int[5]>(11, 22, 33, 44, 55);
#endif
	}
#endif
	
	{
#ifdef ERROR5
		cout << "\n# Reason: maybe souce leak" << endl;
		processWidget(computePriority(),std::shared_ptr<Widget>(new Widget));
#endif
	}
	
	{
#ifdef ERROR6
		//FIXME: destructor does not be called
		cout << "\n# Solution: make_unique" << endl;
		processWidget(computePriority(),std::make_unique<Widget>());
#endif
		//processWidget1(std::make_unique<Widget>(), computePriority());
	}
	
	{
		cout << "\n# circumstances where they can’t or shouldn’t be used: custom deleters" << endl;
		auto widgetDeleter = [](Widget *pw){ delete pw;};
		std::unique_ptr<Widget, decltype(widgetDeleter)> 
			upw(new Widget, widgetDeleter);
		std::shared_ptr<Widget> spw(new Widget, widgetDeleter);
	}
	
	{
		cout << "\n# circumstances where they can’t or shouldn’t be used: braced initializers" << endl;
		auto upv = std::make_unique<std::vector<int> >(10, 20);
		dump(*upv);
		auto spv = std::make_unique<std::vector<int> >(10, 20);
		dump(*spv);

		// create std::initializer_list
		auto initList = {10, 20};
		auto upv1 = std::make_unique<std::vector<int> >(initList);
		dump(*upv1);
		// create std::vector using std::initializer_list ctor
		auto spv1 = std::make_shared<std::vector<int>>(initList);
		dump(*spv1);
	}
	
	{
		cout << "\n# a lag can occur between when an object is destroyed "
			"and when the memory it occupied is freed" << endl;
		auto pBigObj = std::make_shared<ReallyBigType>();
		/**
			// create std::shared_ptrs and std::weak_ptrs to
			// large object, use them to work with it
			
			// final std::shared_ptr to object destroyed here,
			// but std::weak_ptrs to it remain
			
			// during this period, memory formerly occupied
			// by large object remains allocated
			
			// final std::weak_ptr to object destroyed here;
			// memory for control block and object is released
		 */
	}
	return 0;
}
