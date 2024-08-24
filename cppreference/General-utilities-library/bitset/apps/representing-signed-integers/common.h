#ifndef CODE_COMMON_H_
#define CODE_COMMON_H_
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

using bit = bool;
using word = int8_t;

constexpr bit BIT_0 = 0;
constexpr bit BIT_1 = 1;
constexpr size_t WORD_LEN = sizeof(word) * 8;
constexpr size_t TRUE_VALUE_LEN = WORD_LEN - 1;
constexpr size_t SIGN_INDEX = TRUE_VALUE_LEN;

using bin = std::bitset<WORD_LEN>;

std::pair<bit, bit> fake_add(bit a, bit b, bit c);

std::pair<bin, bit> fake_add(bin a, bin b); 

bin word_to_machine(word n);

// 求原码：短除法
bin word_to_original_code(word n);

// 求反码算法1：原码 + 数值部分按位反转(负数)
bin word_to_ones_complement(word n);

// 求反码算法2：绝对值按位反转（负数）
bin word_to_ones_complement1(word n);

// 求补码
bin word_to_twos_complement(word n);
bin word_to_twos_complement1(word n);

word twos_complement_to_word(bin); 
word ones_complement_to_word(bin); 
word orig_code_to_word(bin); 

using decode_func_t = word (*)(bin);
extern std::unordered_map<std::string, decode_func_t> decoders;

// 溢出判断:  根据符号位
static inline bool detect_overflow(bit as, bit bs, bit ss)
{
    return ((as == BIT_0) && (bs == BIT_0) && (ss == BIT_1)) ||
        ((as == BIT_1) && (bs == BIT_1) && (ss == BIT_0)); 
}

#endif
