#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <stdlib.h>
#include <list>
#include <algorithm>
#include <array>

using std::endl;
using std::cout;
using std::vector;
using std::string;

template<typename T, typename ...Args>
void drop(T t, Args... rest)
{
}

constexpr int 
pow(int base, int exp) noexcept
{
	return (exp == 0) ? 1: base * pow(base, exp -1);
}

class Point
{
	public:
		constexpr Point(double xVal = 0, double yVay = 0) noexcept
			:x(xVal), y(yVay){}
		constexpr double xValue() const noexcept { return x; }
		constexpr double yValue() const noexcept { return y; }
#if __cplusplus >= 201402
		constexpr
#endif
		void setX(double newX) noexcept { x = newX; }

#if __cplusplus >= 201402
		constexpr
#endif
		void setY(double newY) noexcept { y = newY; }
	private:
		double x, y;
};

constexpr Point 
midpoint(const Point& p1, const Point& p2) noexcept
{
	return {(p1.xValue() + p2.xValue()) / 2, (p1.yValue() + p2.yValue()) / 2};
}

#if __cplusplus >= 201402
constexpr Point
reflection(const Point& p) noexcept
{
	Point result;

	result.setX(-p.xValue());
	result.setY(-p.yValue());

	return result;
}
#endif

void foo(Point & p)
{
}

int main(int argc, char * argv[])
{
	cout << "__cplusplus:" << __cplusplus << endl << endl;;
	{
		cout << "# constexpr objects" << endl;
		int sz =10;
		// constexpr auto arraySize1 = sz; // error! sz is not known at compilation
		// std::array<int, sz> data1; // error! same problem

		constexpr auto arraySize2 = 10; //fine, 10 is compile-time constant

		std::array<int, arraySize2> data2;

		drop(sz, arraySize2, data2);
	}

	{
		cout << "\n# const objects" << endl;
		int sz =  0;
		const auto arraySize = sz;

		cout << "sz:" << sz << endl;

		//std::array<int, arraySize> data; // error! arraySize's value 
		//not known at compilation
		drop(sz, arraySize);
	}

	{
		cout << "\n# constexpr functions" << endl;
		int exp = 5;
		constexpr auto numConds = 5;
		std::array<int, pow(3, numConds)> results;

		drop(results, exp);
	}

	{
		cout << "\n# literal types" << endl;
		constexpr Point p1(9.4, 27.7);
		constexpr Point p2(28.8, 5.3);

		constexpr auto mid = midpoint(p1, p2);

		std::array<int, static_cast<int>(mid.xValue() * 10)> data;
#if __cplusplus >= 201402
		constexpr auto reflectedMid = reflection(mid);
#endif
		drop(data, reflectedMid);
		//foo(p1); // error! const discard
	}

	return 0;
}
