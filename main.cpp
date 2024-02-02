#include "bignum.hpp"
#include <iostream>

int main(void) {
    auto p = 0.12_bn;
    std::cout << (0.000169_bn / 13_bn).to_string() << '\n';
    return 0;
}