/** 20210511 15:50, zyj, GuangDong*/
//runtime.cpp--displaying type information at runtime
#include <iostream>
#include <vector>
#include <boost/type_index.hpp>

class Widget{};

template<typename T> // template function to
void f(const T& param); // be called

std::vector<Widget> createVec(); // factory function

int main()
{
	const int theAnswer = 42;
	auto x = theAnswer;
	auto y = &theAnswer;

	std::cout << typeid(x).name() << '\n'; // display types for
	std::cout << typeid(y).name() << '\n'; // x and y

	const auto vw = createVec(); // init vw w/factory return
	if (!vw.empty()) {
		f(&vw[0]); // call f
	}
	return 0;
}

template<typename T>
void f(const T& param)
{
	using std::cout;
	cout << "T = " << typeid(T).name() << '\n'; // show T
	cout << "param = " << typeid(param).name() << '\n'; // show

	using boost::typeindex::type_id_with_cvr;
	// show T
	cout << "T = "
		<< type_id_with_cvr<T>().pretty_name()
		<< '\n';
	// show param's type
	cout << "param = "
		<< type_id_with_cvr<decltype(param)>().pretty_name()
		<< '\n';
}

std::vector<Widget>createVec()
{
	return std::vector<Widget>(15);
}
