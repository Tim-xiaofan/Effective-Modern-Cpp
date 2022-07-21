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
}

template<typename T>
void fwd(T&& param) // accept any argument
{
	f(std::forward<T>(param)); // forward it to f
}

int main(int argc, char * argv[])
{

	{
		cout << "# Braced initializers" << endl;
		f({1, 2, 3});

		//fwd({1, 2, 3}); // error! doesn't compile
	}
	return 0;
}
