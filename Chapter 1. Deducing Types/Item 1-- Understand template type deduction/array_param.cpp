/*20210427 16:32, zyj, GuangDong*/
//array_param.cpp--

#include <iostream>
#include <array>

template<typename T>
void f(T param){}// param is now passed by value

template<typename T>
void fa(T& param); // template with by-reference parameter

// return size of an array as a compile-time constant. (The
// array parameter has no name, because we care only about
// the number of elements it contains.)
template<typename T, std::size_t N = 13> // see info
constexpr std::size_t arraySize(T (&)[N]) noexcept // below on
{ // constexpr
	return N; // and
}

int main(int argc, char *argv[])
{
	const char name[] = "J. P. Briggs"; // name's type is const char[13]
	const char * ptrToName = name; // array decays to pointer
	f(name);// T is deduced to be const char*
	f(name); // pass array to f, T is the actual type of the array!

	//std::cout << "array size is " << arraySize(name) << std::end;
	int keyVals[] = { 1, 3, 7, 9, 11, 22, 35 }; // keyVals has 7 elements
	int mappedVals[arraySize(keyVals)]; // so does mappedVals
	std::array<int, arraySize(keyVals)> mappedVals1; // mappedVals' size is 7
	
	return 0;
}
