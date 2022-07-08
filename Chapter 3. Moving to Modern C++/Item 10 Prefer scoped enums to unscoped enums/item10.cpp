#include <iostream>
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include <string>
#include <atomic>
#include <stdlib.h>
#include <mutex>
#include <list>
#include <type_traits>
#include <tuple>

using std::endl;
using std::cout;
using std::unique_ptr;
using std::shared_ptr;
using std::vector;
using std::unordered_map;
using std::string;

std::vector<std::size_t> primeFactors(std::size_t x)
{
	return {0, 1, 2, 3};
}

/** cast enum to it's underlying type*/
template<typename E>
constexpr typename std::underlying_type<E>::type
toUType(E enumerator) noexcept
{
	return static_cast<typename std::underlying_type<E>::type>(enumerator);
}

int main(int argc, char * argv[])
{
	{
		cout << "# enum" << endl;
		enum Color 
		{ black, white, red}; // black, white, red are in same scope as Color
		//auto white = false; // error
		Color c = red;
		if(c < 14.5)
		{
			cout << "c < 14.5" << endl;
			primeFactors(c);
		}
		cout << "sizeof(enum Color):" << sizeof(enum Color) << endl;
	}

	{
		cout << "\n# enum class" << endl;
		enum class Color: std::uint64_t 
		{black, white, red}; // black, white, red are scoped to Color
		auto white = false; //fine
		//Color c = white; // error
		auto c1 = Color::white;
		auto c2 = Color::white;
		cout << static_cast<uint64_t>(c1) 
			<< static_cast<uint64_t>(c2) 
			<< white << endl;

		if(static_cast<double>(c2) < 14.5) // error! can't compare Color and double
		{
			cout << "c < 14.5" << endl;
			primeFactors(static_cast<std::size_t>(c2));
		}
		cout << "sizeof(enum class Color):" << sizeof(enum Color) << endl;
	}

	{
		using UserInfo = std::tuple<std::string, //name
			std::string, //email
			std::size_t>; //reputation
		UserInfo uInfo = {"Tom", "Tom@qq.com", 9};
		
		{
			enum UserInfoFields { uiName, uiEmail, uiReputation };
			auto val = std::get<uiEmail>(uInfo);
			cout << "val=" << val << endl;
		}
		
		{
			enum class UserInfoFields { uiName, uiEmail, uiReputation };
			auto val = std::get<static_cast<std::size_t>(UserInfoFields::uiEmail)>
				(uInfo);
			cout << "val=" << val << endl;
		}

		{
			enum class UserInfoFields { uiName, uiEmail, uiReputation };
			auto val = std::get<toUType(UserInfoFields::uiEmail)>(uInfo);
			cout << "val=" << val << endl;
		}
	}
	return 0;
}
