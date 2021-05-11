/** 202105011 12:51, zyj, GuangDong*/
//universal.cpp -- accept both lvalues and rvalues
#include <iostream>
#include <deque>

using std::cout;
using std::endl;

template<typename Container, typename Index> // final
decltype(auto) // C++14
authAndAccess(Container&& c, Index i) // version
{
	//authenticateUser();
	return std::forward<Container>(c)[i];
}

template<typename Container, typename Index> // final
auto // C++11
authAndAccess1(Container&& c, Index i) // version
-> decltype(std::forward<Container>(c)[i])
{
	//authenticateUser();
	return std::forward<Container>(c)[i];
}

int main()
{
	return 0;
}
