#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <stdlib.h>
#include <list>
#include <algorithm>

using std::endl;
using std::cout;
using std::vector;
using std::string;

#if __cplusplus <= 199711
int f(int x) throw() 
{
	return -1;
} //  no exceptions from f: C++98 style, less optimizable
#else
int f(int x) noexcept 
{
	return -1;
} // no exceptions from f: C++11 style, most optimizable
#endif

class Widget {};

int main(int argc, char * argv[])
{
	cout << "__cplusplus:" << __cplusplus << endl;
	
	std::vector<Widget> vw;
	Widget w;
	vw.push_back(w);
	return 0;
}
