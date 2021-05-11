/** 202105011 09:30, zyj, GuangDong*/
//template_ret.cpp
#include <iostream>
#include <deque>

using std::cout;
using std::endl;

template<typename Container, typename Index> // works, but
auto authAndAccess(Container& c, Index i) // requires
-> decltype(c[i]) // refinement
{
	//authenticateUser();
	return c[i];
}

template<typename Container, typename Index> // C++14;
auto authAndAccess1(Container& c, Index i) // not quite
{ // correct
	//authenticateUser();
	return c[i]; // return type deduced from c[i]
}

template<typename Container, typename Index> // C++14; works,
decltype(auto) // but still
authAndAccess2(Container& c, Index i) // requires
{ // refinement
	//authenticateUser();
	return c[i];
}

int main()
{
	std::deque<int> d;
	std::cout << d[5] << endl;
	//authAndAccess1(d, 5) = 10; // authenticate user, return d[5],
	//then assign 10 to it;
	//this won't compile!
	authAndAccess2(d, 5) = 14;
	std::cout << d[5] << endl;
	return 0;
}
