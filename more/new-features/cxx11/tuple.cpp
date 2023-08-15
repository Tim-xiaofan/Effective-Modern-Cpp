#include <iostream>
#include <tuple>

/** C++11 标准中引入了 std::tuple 类型，有了这个类型，我们就可以放任意个元素了，
  原来需要定义成结构体的 POD 对象我们可以直接使用 std::tuple 表示，例如下面表示用户信息的结构体
  */

struct UserInfo
{
    std::string username;
    std::string password;
    int         gender;
    int         age;
    std::string address;
};

int main(void)
{
	{
		UserInfo userInfo = { "Tom", "123456", 0, 25, "Pudong Street" };
		std::string username = userInfo.username;
		std::string password = userInfo.password;
		int gender = userInfo.gender;
		int age = userInfo.age;
		std::string address = userInfo.address;
		(void) username; (void) password; (void)gender; (void)age; (void)address;
	}

	// 替代POD
	{
		std::tuple<std::string, std::string, int, int, std::string> userInfo("Tom", "123456", 0, 25, "Pudong Street");

		std::string username = std::get<0>(userInfo);
		std::string password = std::get<1>(userInfo);
		int gender = std::get<2>(userInfo);
		int age = std::get<3>(userInfo);
		std::string address = std::get<4>(userInfo);
		(void) username; (void) password; (void)gender; (void)age; (void)address;
	}

	// std::tuple_size<std::tuple>
	{ //Provides access to the number of elements in a tuple as a compile-time constant expression.
		auto value = std::make_tuple(1, 2, 3.14);
		int a[std::tuple_size<decltype(value)>::value]; // can be used at compile time

		std::cout << std::tuple_size<decltype(value)>{} << ' ' // or at run time
			<< sizeof a << ' '
			<< sizeof value << '\n';
	}
	return 0;
}
