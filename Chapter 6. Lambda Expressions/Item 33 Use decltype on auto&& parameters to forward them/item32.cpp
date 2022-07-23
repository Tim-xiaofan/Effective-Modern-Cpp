#include <iostream>

using std::endl;
using std::cout;
using std::cerr;

template<typename T>
T&& normalize(T&& t)
{
	return std::forward<T>(t);
}

template<typename T>
T&& func(T&& t)
{
	return std::forward<T>(t);
}

int main(int argc, char * argv[])
{
	{
		cout << "# C++14 generic lambdas" << endl;
		auto f = [](auto&& x)
			{ return func(normalize(std::forward<decltype(x)>(x)));};
		auto f1 = [](auto&&... x)
			{ return func(normalize(std::forward<decltype(x)>(x)...));};
		f(0);
		f1(0);
	}
	return 0;
}
