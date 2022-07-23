#include <iostream>
#include <memory>
#include <vector>
#include <functional>

using std::endl;
using std::cout;
using std::cerr;

class Widget
{
	public:
		bool isValidated() const {return false;}
		bool isProcessed() const {return false;}
		bool isArchived() const {return false;}
};

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

class IsValAndArch
{
	public:
		using DataType = std::unique_ptr<Widget>;

		explicit IsValAndArch(DataType&& ptr)
			:pw(std::move(ptr)){}

		bool operator()() const
		{
			return pw->isValidated() && pw->isArchived();
		}

	private:
		DataType pw;
};
#endif

//gcc version 11.2.0
int main(int argc, char * argv[])
{
	cout <<"__cplusplus:" << __cplusplus << endl << endl;

#if __cplusplus >= 201402
	{//c++14
		cout << "# c++14 use init capture" << endl;
		auto pw = std::make_unique<Widget>();
		auto func = [pw = std::move(pw)]
		{ return pw->isValidated() && pw->isArchived();};
		cout << "func:" << std::boolalpha << func() << endl;
	}

	{
		cout << "\n# c++14 move into a closure" << endl;
		std::vector<double> data{0.21, 3.14};
		auto func = [data = std::move(data)]{ cout << data[0] << endl;};
		cout << "sizeof(func):" << sizeof(func) << endl;//24
		func();
	}
#endif

#if __cplusplus == 201103
	{
		cout << "# a class in C++11 that supports "
			"move-initialization of its data members" << endl;
		auto func = IsValAndArch(std::make_unique<Widget>());
		cout << "func:" << std::boolalpha << func() << endl;
	}

	{
		std::vector<double> data{0.21, 3.14};
		cout << "\n# move capture can be emulated in C++11" << endl;
		auto func = std::bind(
					[](const std::vector<double>& data)mutable
					{ cout << data[0] << endl;},
					std::move(data)
					);
		func();
		cout << "sizeof(func):" << sizeof(func) << endl;//32
	}
#endif
	return 0;
}
