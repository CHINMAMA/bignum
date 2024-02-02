#include <vector>
#include <stdint.h>
#include <string>

#ifndef BIGNUM_HPP
#define BIGNUM_HPP

class Bignum
{
public:
    int8_t sign;
    int64_t exp;
    std::vector<int8_t> digits;

    void remove_zeroes();
public:
    Bignum();
    explicit Bignum(const char*);
    Bignum operator-() const;

    Bignum operator+(const Bignum&) const;
    Bignum operator-(const Bignum&) const;
    Bignum operator*(const Bignum&) const;
    Bignum operator/(const Bignum&) const;

    Bignum& operator+=(const Bignum&);
    Bignum& operator-=(const Bignum&);
    Bignum& operator*=(const Bignum&);
    Bignum& operator/=(const Bignum&);

    bool operator<(const Bignum&) const;
    bool operator<=(const Bignum&) const;
    bool operator>(const Bignum&) const;
    bool operator>=(const Bignum&) const;
    bool operator==(const Bignum&) const;
    bool operator!=(const Bignum&) const;

    std::string to_string();
    bool is_zero() const;
};

Bignum operator ""_bn(const char*);

#endif