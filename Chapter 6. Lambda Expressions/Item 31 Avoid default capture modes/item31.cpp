#include <iostream>
#include <vector>
#include <functional>
#include <cstdlib>
#include <algorithm>
#include <time.h>
#include <memory>

using std::endl;
using std::cout;
using std::cerr;

using FilterContainer = std::vector<std::function<bool(int)>>;
FilterContainer filters;

static int computeSomeValue(void)
{
	return (1 + std::rand() % 20);
}

static int computeDivisor(int a, int b)
{
	return (a + b) / 2;
}

static void addDivisorFilter(void)
{
	auto calc1 = computeSomeValue();
	auto calc2 = computeSomeValue();
	auto divisor = computeDivisor(calc1, calc2);

	// divisor ceases to exist when addDivisorFilter returns
#ifdef ERROR1
	filters.emplace_back([&divisor](int value) { return value % divisor;});
#else
	filters.emplace_back([divisor](int value) { return value % divisor;});
#endif
}

static void addDivisorFilter1(void)
{
	static auto calc1 = computeSomeValue();
	static auto calc2 = computeSomeValue();
	static auto divisor = computeDivisor(calc1, calc2);

	filters.emplace_back(
				[=](int value)  //captures nothing!
				{ return value % divisor;} //  refers to above static
				);

	++divisor;
}

template<typename C>
void workWithContainer(const C& container)
{
	auto calc1 = computeSomeValue();
	auto calc2 = computeSomeValue();
	auto divisor = computeDivisor(calc1, calc2);
	cout << "divisor=" << divisor << endl;

	using ContElemT = typename C::value_type;
	using std::begin;
	using std::endl;

	if(std::all_of(begin(container), end(container),
					[&](const ContElemT& value)
					{return value % divisor == 0;})
	  )
	{
		cout << "all" << endl;
	}
	else
	{
		cout << "not all" << endl;
	}
}

class Widget
{
	public:
		void addFilter() const
		{
#ifdef ERROR2
#error "capture this pointer"
			filters.emplace_back(
						[=](int value) {return value % divisor == 0;}
						);
#else
#if __cplusplus == 201103
			auto divisorCopy = divisor;
			filters.emplace_back(
						[divisorCopy](int value) { return value % divisorCopy == 0;}
						);
#elif __cplusplus > 201103
			filters.emplace_back(
						[divisor=divisor](int value) {return value % divisor == 0;}
						);
#else
#error "comiler must support stdc++ >= c++11"
#endif
#endif
		}

		~Widget(){ cout << "Widget::destructor" << endl; }

	private:
		int divisor;
};

void doSomeWork()
{
	auto pw = std::make_shared<Widget>();
	pw->addFilter();
}// destroy Widget; filters now holds dangling pointer!

int main(int argc, char * argv[])
{
	std::srand(time(nullptr));

	{
		cout << "# lambda capture by-value and by-referece" << endl;
		int x = 0;
		int y = 42;
		auto qqq = [x, &y] {
			std::cout << "x: " << x << std::endl;
			std::cout << "y: " << y << std::endl;
			++y;
		};
		x = y = 77;
		qqq();
		qqq();
		std::cout << "final y: " << y << std::endl;
	}

	{
		cout << "\n# danger of default byreference capture" << endl;
		filters.emplace_back([](int value) { return value % 5 == 0;});
		addDivisorFilter();
		std::vector<int> vi = {0, 1, 2};
		workWithContainer(vi);
	}

	{
		cout << "\n# lambda capture with class non-static data member" << endl;
		doSomeWork();
	}

	{
		cout << "\n# static storage duration" << endl;
		addDivisorFilter1();
	}
	return 0;
}
