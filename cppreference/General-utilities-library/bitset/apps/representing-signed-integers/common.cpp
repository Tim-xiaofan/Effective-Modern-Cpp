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

static_assert(std::endian::native == std::endian::little, "Only little endian");
// little endian
// human num: 1011
// cpu   num: 1101 low->hign

std::pair<bit, bit> fake_add(bit a, bit b, bit c)
{
    bit sum = BIT_0;
    bit carry = BIT_0;

    // 计算 sum = a + b + c
    int count = BIT_0;

    if (a) ++count;
    if (b) ++count;
    if (c) ++count;

    if (count == 1) {
        sum = BIT_1;
    } else if (count == 2) {
        sum = BIT_0;
        carry = BIT_1;
    } else if (count == 3) {
        sum = BIT_1;
        carry = BIT_1;
    }

    return std::make_pair(sum, carry);
}

std::pair<bin, bit> fake_add(bin a, bin b) {
    bin sum;
    bit carry = BIT_0;

    for (size_t i = 0; i < WORD_LEN; ++i) {
        auto [s, c] = fake_add(a[i], b[i], carry);
        sum[i] = s;
        carry = c;
    }

    return {sum, carry};
}

bin word_to_machine(word n) 
{
    bin result;
    for (size_t i = 0; i < WORD_LEN; ++i) {
        result.set(i, (n & (1 << i)));
    }
    return result;
}

// 求原码：短除法
bin word_to_original_code(word n) 
{
    bin result;

    // 获取符号位：如果 n 是负数，符号位为 '1'，否则为 '0';
    result.set(SIGN_INDEX, (n < 0));

    // 处理数值部分：取绝对值，然后将其转换为 bitset
    n = std::abs(n); 
    for (size_t i = 0; i < TRUE_VALUE_LEN; ++i) {
        result.set(i, n % 2);
        n /= 2;
    }

    if (n) {
        throw std::overflow_error("word_to_original_code");
    }

    return result;
}

// 求反码算法1：原码 + 数值部分按位反转(负数)
bin word_to_ones_complement(word n)
{
    // 取原码
    bin code = word_to_original_code(n);

    if (n < 0) { 
        // 除符号位其余位取反
        for (size_t i = 0; i < TRUE_VALUE_LEN; ++i) {
            code.flip(i);
        }
    }

    // 正数反码与原码相同
    return code;
}

// 求反码算法2：绝对值按位反转（负数）
bin word_to_ones_complement1(word n)
{
    bin code = bin(std::abs(n));
    return (n >= 0) ? code: ~code;
}

// 求补码算法1：(1)求反码 （2）反码+ 1
bin word_to_twos_complement(word n)
{
    // 取反码
    bin code = word_to_ones_complement(n);

    if (n < 0) { 
        // 反码加1
        std::tie(code, std::ignore) = fake_add(code, bin(1));
    }

    // 正数补码与原码相同
    return code;

}

// 求补码算法2：（1）2^TRUE_VALUE_LEN - abs(n) （2）置符号位为 1
bin word_to_twos_complement1(word n)
{
    if ( n < 0 ) {
        bin code((1 << TRUE_VALUE_LEN) - std::abs(n));
        code.set(SIGN_INDEX, 1);
        return code;
    } else {
        return word_to_original_code(n);
    }
}

// 补码转换为10进制：权重法
word twos_complement_to_word(bin twos)
{
    word n = 0;

    for (size_t i = 0; i < SIGN_INDEX; ++i) {
        n += twos.test(i) ? (1 << i) : 0;
    }

    // 符号权重
    n += -1 * (twos.test(SIGN_INDEX) ? (1 << SIGN_INDEX) : 0);
    
    return n;
}

word orig_code_to_word(bin orig)
{
    word n = 0;
    for(size_t i = 0; i < TRUE_VALUE_LEN; ++i) {
         n += (orig.test(i) ? (1 <<i ):BIT_0);
    }
    return (orig[SIGN_INDEX]) ? -n: n;
}

word ones_complement_to_word(bin ones) 
{
    word n = 0;
    for(size_t i = 0; i < TRUE_VALUE_LEN; ++i) {
         n += (ones.test(i) ? BIT_0: (1 << i));
    }
    return (ones[SIGN_INDEX]) ? -n: n;
}

std::unordered_map<std::string, decode_func_t> decoders {
    {"twos", twos_complement_to_word},
    {"ones", ones_complement_to_word},
    {"orig", orig_code_to_word}
};

