#include <vector>
#include <stdint.h>
#include <string>

#ifndef BIGNUM_HPP
#define BIGNUM_HPP

namespace bignum {
    class BigNum {
    public:
        int8_t sign;
        int64_t exp;
        std::vector<int8_t> digits;

        void remove_zeroes();
    public:
        BigNum();
        explicit BigNum(const char*);
        BigNum operator-() const;

        BigNum operator+(const BigNum&) const;
        BigNum operator-(const BigNum&) const;
        BigNum operator*(const BigNum&) const;
        BigNum operator/(const BigNum&) const;

        BigNum& operator+=(const BigNum&);
        BigNum& operator-=(const BigNum&);
        BigNum& operator*=(const BigNum&);
        BigNum& operator/=(const BigNum&);

        bool operator<(const BigNum&) const;
        bool operator<=(const BigNum&) const;
        bool operator>(const BigNum&) const;
        bool operator>=(const BigNum&) const;
        bool operator==(const BigNum&) const;
        bool operator!=(const BigNum&) const;

        BigNum pow(int) const;
        BigNum trunc(int64_t) const;
        std::string to_string();
        bool is_zero() const;
    };

    BigNum calculate_pi(int64_t);
    BigNum operator ""_bn(const char*);
} // namespace bignum

#endif