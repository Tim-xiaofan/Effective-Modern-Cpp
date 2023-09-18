// C++11 支持的 CAS
#include <atomic>
#include <iostream>

namespace
{
std::atomic<int> ai;
 
int tst_val = 4;
int new_val = 5;
bool exchanged = false;
 
void valsout()
{
    std::cout << "ai = " << ai
	      << "  tst_val = " << tst_val
	      << "  new_val = " << new_val
	      << "  exchanged = " << std::boolalpha << exchanged
	      << '\n';
}
}
 
int main(void)
{
    ai = 3;
    valsout();
 
    // tst_val != ai   ==>  tst_val is modified
    exchanged = ai.compare_exchange_strong(tst_val, new_val);
    valsout();
 
    // tst_val == ai   ==>  ai is modified
    exchanged = ai.compare_exchange_strong(tst_val, new_val);
    valsout();

	std::cout << "\n\n";
	ai = 8;
	while(!(exchanged = ai.compare_exchange_weak(tst_val, new_val)))
	{
		valsout();
	}
	valsout();
	return 0;
}
