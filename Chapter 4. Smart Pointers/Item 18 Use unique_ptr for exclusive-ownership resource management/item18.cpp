#include <iostream>
#include <memory>

using std::endl;
using std::cout;
using std::cerr;

enum class type: std::uint8_t {Investment, Stock, Bond, RealEstate};
class Investment 
{
	public:
		virtual constexpr const char * type(void)
		{
			return "Investment";
		}
		virtual ~Investment() {}
};
class Stock: public Investment 
{
	public:
		constexpr const char * type(void) override
		{
			return "Stock";
		}
		virtual ~Stock() {}
};
class Bond: public Investment 
{
	public:
		constexpr const char * type(void) override
		{
			return "Bond";
		}
		virtual ~Bond(){}
};

class RealEstate:public Investment 
{
	public:
		constexpr const char * type(void) override
		{
			return "RealEstate";
		}
		virtual ~RealEstate() {}
};

auto delInvmt = [](Investment * pInvestment)
{
	cout << "delete " << pInvestment->type() << 
		"@" << reinterpret_cast<void*>(pInvestment) << endl;
	delete pInvestment;
};

template <typename... Ts>
std::unique_ptr<Investment, decltype(delInvmt)> 
makeInvestment(enum type t, Ts&&... params)
{
	std::unique_ptr<Investment, decltype(delInvmt)> pInv(nullptr, delInvmt);
	switch(t)
	{
		case type::Stock:
			pInv.reset(new Stock(std::forward<Ts>(params)...));
			break;
		case type::Bond:
			pInv.reset(new Bond(std::forward<Ts>(params)...));
			break;
		case type::RealEstate:
			pInv.reset(new RealEstate(std::forward<Ts>(params)...));
			break;
		default:
			cerr << "Unkown type:" << static_cast<int>(t) << endl;
	}
	return pInv;
}

int main(int argc, char * argv[])
{
	auto pInvestment = makeInvestment(type::Stock);
	std::shared_ptr<int[]> sp(new int[8]);
	cout << sp[7] << endl;
	sp[7] = 125;
	cout << sp[7] << endl;
	return 0;
}
