#include <gtest/gtest.h>
#include "../BigNum/bignum.hpp"

using namespace bignum;

TEST(bignum_operations, add) {
    BigNum val1 = 123.456_bn;
    BigNum val2 = 12.054_bn;
    EXPECT_EQ(135.51_bn, val1 + val2);
}

TEST(bignum_operations, sub) {
    BigNum val1 = 123.456_bn;
    BigNum val2 = 12.054_bn;
    EXPECT_EQ(111.402_bn, val1 - val2);
}

TEST(bignum_operations, mult) {
    BigNum val1 = 123.456_bn;
    BigNum val2 = 12.054_bn;
    EXPECT_EQ(1488.138624_bn, val1 * val2);
}

TEST(bignum_operations, div) {
    BigNum val1 = 126.28935_bn;
    BigNum val2 = 1.023_bn;
    EXPECT_EQ((123.45_bn).to_string(), (val1 / val2).to_string());
}

TEST(calculate_pi, precision_5) {
    BigNum pi = 3.14159_bn;
    EXPECT_EQ(pi, calculate_pi(5));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}