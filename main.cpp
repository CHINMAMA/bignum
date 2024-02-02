#include "bignum.hpp"
#include <iostream>

int main(void) {
    auto p = 0.12_bn;
    std::cout << (235_bn - -122.0982598_bn).to_string() << '\n';
    std::cout << (0.12_bn / 1.2_bn).to_string() << '\n';
    return 0;
}