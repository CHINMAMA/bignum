#include <deque>
#include <stdint.h>
#include <string>

#ifndef BIGNUM_HPP
#define BIGNUM_HPP
#define DEFAULT_PRECISION 15

namespace bignum {
    class BigNum {
    private:
        std::deque<int8_t> digits;
        int64_t precision;
        static inline int64_t default_precision = DEFAULT_PRECISION;
    public:
        int64_t exp;
        int8_t sign;
        int64_t get_precision() const;
        void set_precision(int64_t);
        void remove_zeroes();
        BigNum(int64_t);
        BigNum(const BigNum&);
        explicit BigNum(const char*, int64_t);
        static void set_default_precision(int64_t);
        static int64_t get_default_precision();
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

    BigNum operator ""_bn(const char*);
} // namespace bignum

#endif