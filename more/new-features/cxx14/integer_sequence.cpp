#include <utility>
#include <iostream>
#include <tuple> // since c++11
#include <array>

// The class template std::integer_sequence represents a compile-time sequence of integers. When used as an argument to a function template, 
// the parameter pack Ints can be deduced and used in pack expansion

namespace A
{ // Helper templates

template<std::size_t... Ints>
using index_sequence = std::integer_sequence<std::size_t, Ints...>;

/* Helper alias templates std::make_integer_sequence and std::make_index_sequence are defined to 
	simplify creation of std::integer_sequence and std::index_sequence types, respectively, with 0, 1, 2, ..., N-1 as Ints: */
//template<class T, T N>
//using make_integer_sequence = std::integer_sequence<T, /* a sequence 0, 1, 2, ..., N-1 */ >;

template<std::size_t N>
using make_index_sequence = std::make_integer_sequence<std::size_t, N>;

// The program is ill-formed if N is negative. If N is zero, the indicated type is integer_sequence<T>

// A helper alias template std::index_sequence_for is defined to convert any type parameter pack into an index sequence of the same length
template<class... T>
using index_sequence_for = std::make_index_sequence<sizeof...(T)>;

}

/** 逗号展开*/
template<typename T>
void print_item(T t){
	std::cout << t << ", ";
}

// debugging aid
template<typename T, T... ints>
void print_sequence(std::integer_sequence<T, ints...> int_seq)
{
    std::cout << "The sequence of size " << int_seq.size() << ": ";
	int arr[] = { (print_item(ints), 0)...};
    std::cout << '\n';
	(void) arr;
}

// convert array into a tuple
template<typename Array, std::size_t... I>
auto a2t_impl(const Array& a, std::index_sequence<I...>)
{
    return std::make_tuple(a[I]...);
}

template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
auto a2t(const std::array<T, N>& a)
{
    return a2t_impl(a, Indices{});
}

// pretty-print a tuple
template<class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple_impl(std::basic_ostream<Ch,Tr>& os,
                      const Tuple& t,
                      std::index_sequence<Is...>)
{
    //((os << (Is == 0? "" : ", ") << std::get<Is>(t)), ...);
	int arr[] = { (print_item(std::get<Is>(t)), 0)...};
	(void) arr;
}

template<class Ch, class Tr, class... Args>
auto& operator<<(std::basic_ostream<Ch, Tr>& os,
                 const std::tuple<Args...>& t)
{
    os << "(";
    print_tuple_impl(os, t, std::index_sequence_for<Args...>{});
    return os << ")";
}

int main(void)
{
	print_sequence(std::integer_sequence<unsigned, 9, 2, 5, 1, 9, 1, 6>{});
    print_sequence(std::make_integer_sequence<int, 20>{});
    print_sequence(std::make_index_sequence<10>{});

	std::array<int, 4> array = {1, 2, 3, 4};
	auto tuple = a2t(array);
	// print it to cout
    std::cout << "The tuple " << ": " << tuple << '\n';
	return 0;
}
