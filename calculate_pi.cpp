#include "./BigNum/bignum.hpp"
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <chrono>
using namespace bignum;

BigNum calculate_pi(int64_t precision) {
    BigNum::set_default_precision(precision + 5);
    BigNum arctg1_5 = 0_bn;
    BigNum C1_5 = 0.2_bn;
    BigNum arctg1_239 = 0_bn;
    BigNum C1_239 = 1_bn / 239_bn;
    BigNum big_i = 1_bn;
    BigNum cur1 = C1_5;
    BigNum cur2 = C1_239;
    C1_5 /= -5_bn;
    C1_239 /= -239_bn;
    for (int64_t i = 0; i <= precision; ++i, big_i += 2_bn) {
        BigNum tmp = cur2 / big_i;
        arctg1_239 += tmp;
        arctg1_5 += cur1 / big_i;
        cur1 *= C1_5;
        cur2 *= C1_239;
    }

    BigNum res = 4_bn * (4_bn * arctg1_5 - arctg1_239);
    return res.trunc(precision + res.exp + 1);
}