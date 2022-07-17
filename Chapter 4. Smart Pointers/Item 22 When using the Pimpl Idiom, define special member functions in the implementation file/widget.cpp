#include "widget.h" // in impl. file "widget.cpp"
#include "gadget.h"
#include <string>
#include <vector>
#include <iostream>

using std::cout;
using std::endl;

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


struct Widget::Impl
{
	std::string name;
	std::vector<double> data;
	Gadget g1, g2, g3;
	~Impl()
	{
		std::cout << "Impl::destructor @" << this << std::endl;
	}
	Impl()
	{
		std::cout << "Impl::constructor @" << this << std::endl;
	}
};

Widget::Widget()
		:pImpl(std::make_unique<struct Widget::Impl>())
{
	std::cout << "Widget::constructor @" << this << std::endl;
}

Widget::~Widget()
{
	std::cout << "Widget::destructor @" << this << std::endl;
}


Widget::Widget(Widget&& rhs)
		:pImpl(std::move(rhs.pImpl))
{
	cout << "Widget::move constructor" << endl;
}

Widget& Widget::operator=(Widget&& rhs)
{
	cout << "Widget::move assignment" << endl;
	if(&rhs != this)
	{
		pImpl = std::move(rhs.pImpl);
	}
	return *this;
}

Widget::Widget(const Widget & rhs)
		:pImpl(std::make_unique<Impl>(*rhs.pImpl))
{
}

Widget& Widget::operator=(const Widget& rhs)
{
	*pImpl = *rhs.pImpl;
	return *this;
}
