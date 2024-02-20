#include "./BigNum/bignum.hpp"
#include "calculate_pi.cpp"
#include <iostream>
#include <chrono>

using namespace bignum;

int main(void) {
    auto begin = std::chrono::steady_clock::now();
    std::cout << (calculate_pi(100)).to_string() << '\n';
    auto end = std::chrono::steady_clock::now();

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << elapsed_ms.count() << '\n';

    return 0;
}

