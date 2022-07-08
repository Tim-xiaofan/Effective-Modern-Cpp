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

#if __cplusplus == 199711
#define STD_98
#elif __cplusplus == 201103
#define STD_11
#elif __cplusplus > 201103
#define STD_14
#endif

template<typename C>
void dump_container(const C& c, 
			const string& info = "", 
			const string& delim = " ",
			const string& end ="\n")
{
	cout << info;
	for(typename C::const_iterator it = c.begin(); it != c.end(); ++it)
	{
		cout << *it << delim; 
	}
	cout << end;
}


#ifndef STD_98
template<typename T>
void dump_array(const T* a, int n, 
			const string& info ="", 
			const string& delim = " ",
			const string& end="\n")
{
	cout << info;
	for(int i = 0; i < n; ++i)
	{
		cout << a[i] << delim;
	}
	cout << end;
}
#ifdef STD_11
namespace std
{
	template<class C>
	auto cbegin(const C & container) -> decltype(std::begin(container))
	{
		return std::begin(container);
	}
	
	template<class C>
	auto cend(const C & container) -> decltype(std::end(container))
	{
		return std::end(container);
	}
}
#endif

template <typename C, typename V>
void findAndInsert(C& container, const V& targetVal, const V& insertVal)
{
	using std::cbegin;
	using std::cend;
	
	auto it = std::find(cbegin(container), cend(container), targetVal);
	container.insert(it, insertVal);
}
#endif

int main(int argc, char * argv[])
{
	cout << "__cplusplus:" << __cplusplus << endl << endl;
	{
		cout << "# non-const iterator" << endl;
		vector<int> values;
		values.push_back(1981);
		values.push_back(1982);
		values.push_back(1983);
		values.push_back(2022);
		dump_container(values, "orig:");
		vector<int>::iterator it =
			std::find(values.begin(), values.end(), 1983);
		values.insert(it, 1998);
		dump_container(values, "insert:");
	}

	{
		cout << "\n# const iterator" << endl;
#ifdef STD_98
		typedef std::vector<int>::iterator IterT;
		typedef std::vector<int>::const_iterator ConstIterT;
#else
		using ConstIterT = std::vector<int>::const_iterator;
#endif
		vector<int> values;
		values.push_back(1981);
		values.push_back(1982);
		values.push_back(1983);
		values.push_back(2022);
		dump_container(values, "orig:");
#ifdef STD_98
		ConstIterT ci = 
			std::find(static_cast<ConstIterT>(values.begin()),
						static_cast<ConstIterT>(values.end()), 1983);
		values.insert(static_cast<IterT>(ci), 1998); // error
#else
		ConstIterT ci = std::find(values.cbegin(), values.cend(), 1983);
		values.insert(ci, 1998);
#endif
		dump_container(values, "insert:");
	}

#ifndef STD_98
	{
		cout << "\n# findAndInsert" << endl;
		vector<int> values = {1981, 1982, 1983, 2022};
		dump_container(values, "orig:");
		findAndInsert(values, 1983, 1998);
		dump_container(values, "insert:");
	}

	{
		cout << "\n# begin and end:built-in array" << endl;
		int values[5] = {1981, 1982, 1983, 2022};
		dump_array(values, 4, "orig:");
		cout << (void *)std::cbegin(values) << endl;;
		cout << (void *)std::cend(values) << endl;;
	}
#endif
	return 0;
}
