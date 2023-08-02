#include <iostream>
#include <string>
#include <memory>
#include <type_traits>
#include <array>
#include <utility>
#include <iomanip>

using std::cout;
using std::endl;
using std::array;

namespace A 
{
template <auto... seq>
struct my_integer_sequence
{
};

template <typename... Args>
bool logicalAnd(Args... args) {
	// Binary folding
	return (true && ... && args);
}
}

/**
  C++11引入了可变参数模板（variadic template），它可以接收任意数量的模板参数，
  但是参数包不能直接展开，需要通过递归或者逗号表达式的方式进行展开，写法非常繁琐

  C++17对这个问题进行了优化，引入了折叠表达式的概念，用来简化对可变参数模板中参数包的展开过程
 */


/** 递归展开*/
void print(void){
	cout << endl;
}

template<typename T>
void print(T t){
	cout << t << endl;
}

template <typename T, typename... Args>
void print(T t, Args... args){
	 cout << t << ",";
	 print(args...);
}

/** 逗号展开*/
template<typename T>
void print_item(T t){
	cout << t << ",";
}

template<typename... T>
void expand(T... args) {
    int arr[] = { (print_item(args), 0)... }; //逗号表达式
    std::cout << std::endl;
	//for(size_t i = 0; i < sizeof(arr)/sizeof(int); ++i){
	//	cout << "arr[" << i << "]:"<< arr[i] << endl;
	//}
	(void)arr;
}



//折叠表达式有4种语法形式（括号也是折叠表达式的一部分）
/**
  (pack op ...) //一元右折叠

  (... op pack) //一元左折叠

  (pack op ... op init) //二元右折叠

  (init op ... op pack) //二元左折叠

  */

//一元右折叠
template<typename... T>
auto sumR(T... args) {
    return (args + ...); // 一元右折叠展开之后的含义是(E1 op (... op (En-1 op En)))。
}

auto sumR() {
	return 0;
}

// op是逗号,，E是输出语句std::cout << args << ", "
template<typename... T>
auto printR(T... args) {
	((std::cout << args << ", "), ...) << std::endl;
}

//一元左折叠
template<typename... T>
auto sumL(T... args) {
	return (... + args); //(((E1 op E2) op ...) op En)
}

auto sumL() {
	return 0;
}

template<typename... T>
auto printL(T... args) {
	(..., (std::cout << args << ", ")) << std::endl;
}


/// 二元折叠
//二元右折叠表达式
template<typename... T>
auto sumR1(T... args) {
	return (args + ... + 0);
}

//二元左折叠表达式
template<typename... T>
auto sumL1(T... args) {
	return (0 + ... + args);
}


/// 注意事项
// 从累加求和的一元折叠表达实现中我们得到结论，累加时一元折叠表达式不支持空参数的参数包展开，必须实现一个特化版本
// 但是对于一元折叠表达式，以下三种操作符可以直接支持空参数的参数包展开
// &&，逻辑与。空包的值代表true。
// ||，逻辑或。空包的值代表false
// ,，逗号运算符。空包的值代表void()

template<typename... T>
auto all(T... args) {
	return (... && args);
}

template<typename... T>
auto any(T... args) {
	return (args || ...);
}

template<typename T, typename... Args>
auto for_each(T func, Args... args) {
	((func(args)) , ...);
}

int main(void)
{
	{// 用auto声明非类型模板参数
		// 依据auto的推到规则，在可允许类型的非类型模板参数情况下，可从实参类型推导出模板参数
		auto seq1 = std::integer_sequence<int, 0, 1, 2>();
		auto seq2 = A::my_integer_sequence<0, 1, 2>();
		//auto seq3 = A::my_integer_sequence<0, 1, 3.2>(); // error
		(void)seq1;
		(void)seq2;
		//(void)seq3;
	}

	{
		print("string");
		print(1);
		print(1, 1.0, "abcd");
		
		expand("string");
		expand(1);
		expand(1, 1.0, "abcd");
	}

	{// 一元折叠
		std::cout << std::endl;
		printL(1, 2, 3, 4);
		print(sumL(1, 2, 3, 4));
		std::cout << std::endl;

		printL(std::string("1"), "2", "3", "4");
		print(sumL(std::string("1"), "2", "3", "4"));
		std::cout << std::endl;

		printR(1, 2, 3, 4);
		print(sumR(1, 2, 3, 4));
		std::cout << std::endl;

		printR("1", "2", "3", std::string("4"));
		print(sumR("1", "2", "3", std::string("4")));
		std::cout << std::endl;
	}

	{// 二元折叠
		std::cout << std::endl;
		print(1, 2, 3, 4);
		print(sumR1(1, 2, 3, 4));
		std::cout << std::endl;

		print(1, 2, 3, 4);
		print(sumL1(1, 2, 3, 4));
	}

	{
		std::cout << "all(): " << std::boolalpha << all() << std::endl;
		std::cout << "all(1, 2): " << std::boolalpha << all(1, 2) << std::endl;
		std::cout << "all(1, 0): " << std::boolalpha << all(1, 0) << std::endl;
	}

	return 0;
}
