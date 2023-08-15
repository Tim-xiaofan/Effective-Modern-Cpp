#include <iostream>
#include <tuple>
#include <string>
#include <type_traits>
#include <vector>

// helper function to print a vector to a stream
template<class Os, class T>
Os& operator<< (Os& os, std::vector<T> const& v)
{
	int i = v.size();
    os << '{';
    for (const T& e : v)
        os << e << (--i ? "," : "");
    return os << '}';
}

template<class T>
void print_single(T const& v)
{
	std::cout << v;
}

// helper function to print a tuple of any size
template<class Tuple, std::size_t N>
struct TuplePrinter
{
    static void print(const Tuple& t)
    {
        TuplePrinter<Tuple, N - 1>::print(t);
        std::cout << ", ";
        print_single(std::get<N - 1>(t));
    }
};

template<class Tuple>
struct TuplePrinter<Tuple, 1>
{
    static void print(const Tuple& t)
    {
        print_single(std::get<0>(t));
    }
};

template<class... Args>
void print(const std::string& message, const std::tuple<Args...>& t)
{
	std::cout << message << " (";
    TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
    std::cout << ")\n";
}

int main(void)
{
	// 创建一个空的元组， 创建时，需要指定元组的数据类型
	std::tuple<int, std::string, double> t1;
	print("Value-initialized, t1:", t1);
	
	decltype(t1) t2{42, "test", -3.14};
	print("Initialized with values, t2:", t2);

	std::tuple<char, std::string, int> t3{t2};
    print("Implicitly converted, t3:", t3);

	std::tuple<int, double> t4{std::make_pair(42, 3.14)};
    print("Constructed from a pair, t4:", t4);

	// given Allocator my_alloc with a single-argument constructor
    // my_alloc(int); use my_alloc(1) to allocate 5 ints in a vector
    using my_alloc = std::allocator<int>;
    std::vector<int, my_alloc> v{5, 1, my_alloc{/* 1 */}};

	// use my_alloc(2) to allocate 5 ints in a vector in a tuple
    std::tuple<int, std::vector<int, my_alloc>, double> t5 {std::allocator_arg, my_alloc{/* 2 */}, 42, v, -3.14};
    print("Constructed with allocator, t5:", t5);

	return 0;
}
