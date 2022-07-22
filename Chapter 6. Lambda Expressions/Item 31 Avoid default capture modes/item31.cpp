#include <iostream>
#include <vector>
#include <functional>
#include <cstdlib>

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

	// ceases to exist when addDivisorFilter returns
	filters.emplace_back([&divisor](int value) { return value % divisor;});
}

int main(int argc, char * argv[])
{
	{
		cout << "# danger of default byreference capture" << endl;
		filters.emplace_back([](int value) { return value % 5 == 0;});
		addDivisorFilter();
	}
	return 0;
}
