#include "BigNum/bignum.hpp"
#include <iostream>

using namespace bignum;

int main(void) {
    std::cout << (calculate_pi(10)).to_string() << '\n';

    return 0;
}

