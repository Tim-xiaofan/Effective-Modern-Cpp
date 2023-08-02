#include <iostream>
#include <string>
#include <memory>
#include <type_traits>
#include <array>
#include <utility>
#include <iomanip>
#include <cstring>
#include <string_view>

using std::cout;
using std::endl;
using std::array;
using namespace std::string_view_literals;

int main(void)
{
	{ // C风格字符串初始化方式
		const char* arr = "cstring";
		char arr1[15] = "cstring";
		char arr2[] = { 'c', 's', 't', 'r', 'i', 'n',  'g', '\0' }; //结尾必须有\0结束符

		//C风格字符串函数
		(void)strlen(arr);
		(void)strcmp(arr1, arr2);
		(void)strcat(arr1, arr2);
		(void)strcpy(arr1, arr2);
	}

	{ // C++ std::string对象
		//初始化方式
		std::string s1;
		std::string s2(s1);
		std::string s3 = s1;
		std::string s4("world");
		std::string s5 = "world";
		std::string s6 = std::string("world");
		std::string s7(5, 'a'); //aaaaa
	}

	///	在C++17之前，可以通过C风格字符串指针作为函数形参，也可以通过std::string字符串引用类型作为函数形参。但是这并不完美，从实践上看，存在以下问题：
	/// 
	/// C风格字符串的传递仍会进行拷贝
	/// 字符数组、字符串字面量和字符串指针是可以隐式转换为std::string对象的，当函数的形参是std::string，而传递的实参是C风格字符串时，编译器会做一次隐式转换，生成一个临时的std::string对象，再让形参指向这个对象。字符串字面值一般较小，性能消耗可以忽略不计；但是字符数组和字符串指针往往较大，频繁的数据拷贝就会造成较大的性能消耗，不得不重视。
	/// 
	/// substr()的复杂度是O(N)
	/// std::string提供了一个返回字符串字串的函数，但是每次返回的都是一个新的对象，也需要进行构造

	/// C++17 string_view
	{
		std::string_view sv("hello, world");
		std::cout << "sv = " << sv
			<< ", size() = " << sv.size()
			<< ", data() = " << sv.data() << std::endl;

		std::string_view sv2 = sv.substr(0, 5); // O(1)
		std::cout << "sv2 = " << sv2
			<< ", size() = " << sv2.size()
			<< ", data() = " << sv2.data() << std::endl;

		std::string_view sv3 = "hello\0 world"sv;//或std::string_view sv4("hello\0 world"sv)
		std::cout << "sv3 = " << sv3
			<< ", size() = " << sv3.size()
			<< ", data() = " << sv3.data() << std::endl;

		std::string_view sv4("hello\0 world");
		std::cout << "sv4 = " << sv4
			<< ", size() = " << sv4.size()
			<< ", data() = " << sv4.data() << std::endl;
	}
	{ // data(): 返回的是起始位置的字符指针，若以其返回值进行输出打印，会一直输出直到遇到\0结束符
		std::cout << std::endl;
		std::string_view sv("Hello World");
		std::cout << sv.data() << std::endl;
		std::string_view sv1 = sv.substr(0, 5);
		std::cout << "sv1: " << sv1 << std::endl;
		std::cout << "but sv1.data(): " << sv1.data() << std::endl;
	}
	{ //容量
		std::cout << std::endl;
		std::string_view sv;
		std::cout << "size(): " << sv.size() << std::endl;
		std::cout << "length(): " << sv.length() << std::endl;
		std::cout << "max_size(): " << sv.max_size() << std::endl;
		std::cout << "empty(): " << sv.empty() << std::endl;
	}
	{// 修改器
		std::cout << std::endl;
		std::string str = "   hello";
		std::string_view v = str;
		v.remove_prefix(std::min(v.find_first_not_of(" "), v.size()));
		std::cout << "String: '" << str << "', View  : '" << v  << "'" << std::endl; //String: '   hello', View  : 'hello'
		std::cout << "size(): " << v.size() << std::endl;
		v.remove_suffix(1);
		std::cout << "String: '" << str << "', View  : '" << v  << "'" << std::endl; //String: '   hello', View  : 'hello'
		std::cout << "size(): " << v.size() << std::endl;

		std::string_view v1("I'm v1");
		std::string_view v2("I'm v2");
		v1.swap(v2);
		std::cout << "v1: " << v1 << std::endl;
		std::cout << "v2: " << v2 << std::endl;
	}
	{ // 拷贝
		/// 拷贝子串[pos, pos + rcount)到dest所指向的字符序列中，其中rcount = min(count, size() - pos)。
		/// 如果pos > size()，则会抛出std::out_of_range异常
		std::cout << std::endl;
		constexpr std::basic_string_view<char> source{"ABCDEF"};
		std::array<char, 8> dest;
		std::size_t count{}, pos{};

		dest.fill('\0');
		source.copy(dest.data(), count = 4); // pos = 0
		std::cout << dest.data() << '\n'; // ABCD

		dest.fill('\0');
		source.copy(dest.data(), count = 4, pos = 1);
		std::cout << dest.data() << '\n'; // BCDE

		dest.fill('\0');
		source.copy(dest.data(), count = 42, pos = 2); // ok, count -> 4
		std::cout << dest.data() << '\n'; // CDEF

		try
		{
			source.copy(dest.data(), count = 1, pos = 666); // throws: pos > size()
		}
		catch (std::out_of_range const& ex)
		{
			std::cout << ex.what() << '\n';
		}
	}

	{// substr
		/// 返回子串[pos, pos + rcount)的视图，其中rcount = min(count, size() - pos)，不同于std::string::substr()的时间复杂度O(n)，它的时间复杂度是O(1)。
		/// 如果pos > size()，则会抛出std::out_of_range异常
		std::cout << std::endl;
		std::string_view sv("Hello World");
		std::string_view sv1 = sv.substr(0, 5);
		std::cout << sv1 << std::endl;

		try
		{
			sv.substr(60, 7);
		}
		catch (const std::out_of_range& ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}
	return 0;
}
