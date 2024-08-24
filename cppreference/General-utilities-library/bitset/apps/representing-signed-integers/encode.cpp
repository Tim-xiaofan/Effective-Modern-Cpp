#include <string>
#include <bitset>
#include <iostream>
#include <tuple>
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <bitset>
#include <bit>
#include <sstream>

#include "common.h"

static void test(word num)
{
    bin mac;
    bin ones;
    bin ones1;
    bin twos;
    bin twos1;
    std::cout << "数    : " << static_cast<int>(num) << std::endl;
    std::cout << "原码  ： " << word_to_original_code(num) << std::endl;
    std::cout << "反码  ： " << (ones=word_to_ones_complement(num))<< std::endl;
    std::cout << "反码1 ： " << (ones1=word_to_ones_complement1(num))<< std::endl;
    std::cout << "补码  ： " << (twos = word_to_twos_complement(num)).to_string() << std::endl;
    std::cout << "补码1 ： " << (twos1 = word_to_twos_complement1(num)).to_string() << std::endl;
    std::cout << "机器数： " << (mac = word_to_machine(num)) << std::endl;
    std::cout << "bitset： " << bin(num) << std::endl;
    std::cout << "取反：   "   << bin(num).flip() << std::endl;

    assert(ones == ones1);
    assert(mac == twos);
    assert(mac == twos1);
    assert(mac == bin(num));
}


int main(int argc, char *argv[]) 
{
    if (argc == 2) {
        if (strcmp(argv[1], "-0") == 0) {
            std::cout << "数    : " << "-0" << std::endl;
            std::cout << "原码  ： " << "10000000" << std::endl;
            std::cout << "反码  ： " << "11111111" << std::endl;
            std::cout << "补码  ： " << "00000000" << std::endl;
            std::cout << "bitset： " << "00000000" << std::endl;
            std::cout << "机器数： " << "00000000" << std::endl;
        } else {
            int64_t tmp = std::stoi(argv[1]);
            int64_t min = -(1 << TRUE_VALUE_LEN);
            int64_t max = ((1 << TRUE_VALUE_LEN)-1); 
            if (tmp < min || tmp > max) {
                std::ostringstream oss;
                oss << "Num must in range[" 
                    << min << ", "
                    << max << "]";
                throw std::invalid_argument(oss.str());
            }

            test(static_cast<word>(tmp));
        }
    } else {
        std::cerr << "Usage: " << argv[0] << " num\n";

        return -1;
    }

    std::cout << "All done\n";

    return 0;
}

