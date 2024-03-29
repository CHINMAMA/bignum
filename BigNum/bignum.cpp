#include "bignum.hpp"
#include <string.h>
#include <iostream>
#include <stdexcept>
#define MAX_ITERATIONS 30

namespace bignum {
    BigNum::BigNum() {
        exp = 0;
        sign = 1;
        digits = std::vector<int8_t>(1, 0);
    }

    BigNum::BigNum(const char* str) {
        sign = 1;
        if (str[0] == '-') {
            sign = -1;
        }
        size_t len = strlen(str);
        exp = len - (sign == -1) - 1;
        digits = std::vector<int8_t>(len);
        bool is_frac = false;
        for (int i = sign == 1 ? 0 : 1; str[i]; ++i) {
            if (str[i] != '.') {
                digits[len - i + is_frac - 1] = str[i] - '0';
            }
            else {
                exp = sign == 1 ? i - 1 : i - 2;
                is_frac = true;
            }
        }

        remove_zeroes();
    }

    void BigNum::remove_zeroes() {
        while (digits.back() == 0 && digits.size() > 1) {
            digits.pop_back();
            --exp;
        }

        while (digits.front() == 0 && digits.size() > 1) {
            digits.erase(digits.begin());
        }

        if (digits.size() == 1 && digits[0] == 0) {
            exp = 0;
            sign = 1;
        }
    }

    std::string BigNum::to_string() {
        std::string res = "";
        if (is_zero()) {
            return "0";
        }

        if (sign == -1) {
            res += '-';
        }

        if (exp < 0) {
            res += "0.";
        }

        for (int i = exp + 1; i < 0; ++i) {
            res += '0';
        }

        int64_t len = digits.size();
        for (int64_t i = len - 1; i >= 0; --i) {
            res += digits[i] + '0';
            if (len - i - 1 == exp && i != 0) {
                res += '.';
            }
        }

        for (int i = len - 1; i < exp; ++i) {
            res += '0';
        }

        return res;
    }

    bool BigNum::is_zero() const {
        return digits.size() == 1 && 
            digits[0] == 0;
    }

    BigNum BigNum::operator-() const {
        BigNum res;
        res.exp = exp;
        res.digits = digits;
        res.sign = -sign;
        
        return res;
    }

    bool BigNum::operator<(const BigNum& value) const {
        if (sign != value.sign) {
            return sign < value.sign;
        }

        if (exp != value.exp) {
            if (is_zero()) {
                return true;
            }

            if (value.is_zero()) {
                return false;
            }

            return exp < value.exp;
        }

        
        for (
                auto it1 = digits.rbegin(), it2 = value.digits.rbegin(); 
                it1 != digits.rend() && it2 != value.digits.rend(); 
                it1++, it2++
            ) {
            if (*it1 != *it2) {
                return *it1 < *it2;
            }
        }
        return digits.size() < value.digits.size();
    }

    bool BigNum::operator==(const BigNum& value) const {
        if (sign != value.sign) {
            return false;
        }

        if (exp != value.exp) {
            return false;
        }

        if (digits.size() != value.digits.size()) {
            return false;
        }

        for (
                auto it1 = digits.rbegin(), it2 = value.digits.rbegin(); 
                it1 != digits.rend() && it2 != value.digits.rend(); 
                it1++, it2++
            ) {
            if (*it1 != *it2) {
                return false;
            }
        }

        return true;
    }

    bool BigNum::operator>(const BigNum& value) const {
        return !(*this < value || *this == value);
    }

    bool BigNum::operator!=(const BigNum& value) const {
        return !(*this == value);
    }

    BigNum BigNum::operator+(const BigNum& value) const {
        if (sign > value.sign) {
            return *this - (-value);
        }

        if (sign < value.sign) {
            return value - (-(*this));
        }

        BigNum res;
        int64_t max_exp = std::max(exp, value.exp);
        res.exp = max_exp;
        res.sign = sign;

        int64_t size1 = digits.size();
        int64_t size2 = value.digits.size();
        int64_t frac1 = size1 - exp - 1;
        int64_t frac2 = size2 - value.exp - 1;
        int64_t max_frac = std::max(frac1, frac2);
        int64_t len1 = size1 + (frac2 > frac1 ? frac2 - frac1 : 0);
        int64_t len2 = size2 + (frac1 > frac2 ? frac1 - frac2 : 0);
        int64_t len = std::max(len1, len2);

        int64_t offset1 = max_frac - frac1;
        int64_t offset2 = max_frac - frac2;

        for (int64_t i = 0, carry = 0; i < len || carry; ++i) {
            int8_t val1 = 
                offset1 <= i && i < digits.size() + offset1 ? 
                digits[i - offset1] :
                0;
            int8_t val2 = 
                offset2 <= i && i < value.digits.size() + offset2 ? 
                value.digits[i - offset2] : 
                0;
            int8_t cur = val1 + val2 + carry;
            carry = cur / 10;
            res.digits.push_back(cur % 10);
        }

        res.remove_zeroes();
        return res;
    }

    BigNum BigNum::operator-(const BigNum& value) const {
        if (sign != value.sign) {
            return *this + (-value);
        }

        if (sign == -1 && value.sign == -1) {
            return -value - -(*this);
        }

        bool cmp = *this > value;
        const BigNum& tmp1 = cmp ? *this : value;
        const BigNum& tmp2 = cmp ? value : *this;

        int64_t size1 = tmp1.digits.size();
        int64_t size2 = tmp2.digits.size();
        int64_t frac1 = size1 - exp - 1;
        int64_t frac2 = size2 - tmp2.exp - 1;
        int64_t max_frac = std::max(frac1, frac2);
        int64_t len1 = size1 + (frac2 > frac1 ? frac2 - frac1 : 0);
        int64_t len2 = size2 + (frac1 > frac2 ? frac1 - frac2 : 0);
        int64_t size = std::max(len1, len2);

        int64_t max_exp = std::max(tmp1.exp, tmp2.exp);
        int64_t offset1 = max_frac - frac1;
        int64_t offset2 = max_frac - frac2;

        BigNum res;
        res.exp = max_exp;
        res.sign = cmp ? 1 : -1;

        for (int64_t i = 0, carry = 0; i < size || carry; ++i) {
            int8_t val1 = 
                offset1 <= i && i < tmp1.digits.size() + offset1 ? 
                tmp1.digits[i - offset1] : 
                0;
            int8_t val2 = 
                offset2 <= i && i < tmp2.digits.size() + offset2 ? 
                tmp2.digits[i - offset2] : 
                0;
            
            int8_t cur = val1 - val2 - carry;
            carry = cur < 0;
            if (carry) {
                cur += 10;
            }

            res.digits.push_back(cur);
        }

        res.remove_zeroes();
        return res;
    }

    BigNum BigNum::operator*(const BigNum& value) const {
        BigNum res;
        size_t size = digits.size() + value.digits.size() + 1;
        res.sign = sign * value.sign;
        res.exp = exp + value.exp;
        res.digits = std::vector<int8_t>(size, 0);

        for (int i = 0; i < digits.size(); ++i) {
            for (int j = 0, carry = 0; j < value.digits.size() || carry; ++j) {
                int cur_index = i + j;
                long long cur = 
                    res.digits[cur_index] + 
                    digits[i] * (j < value.digits.size() ? value.digits[j] : 0) + 
                    carry;
                carry = cur / 10;
                res.digits[cur_index] = cur % 10;
                if (i == digits.size() - 1 && j == value.digits.size()) {
                    ++res.exp;
                }
            }
        }

        while (res.digits.back() == 0 && res.digits.size() > 1) {
            res.digits.pop_back();
        }

        res.remove_zeroes();
        return res;
    }

    BigNum BigNum::operator/(const BigNum& value) const {
        if (value.is_zero()) {
            throw std::invalid_argument("Division by zero");
        }
        if (value == 1_bn) {
            return *this;
        }
        BigNum res;
        BigNum dividend(*this);
        BigNum divisor(value);
        res.sign = dividend.sign * divisor.sign;
        dividend.exp += divisor.digits.size() - divisor.exp - 1;
        divisor.exp = divisor.digits.size() - 1;
        
        BigNum t_dividend = dividend;
        t_dividend.exp = 0;
        BigNum t_divisor = divisor;
        t_divisor.exp = 0;

        res.exp = dividend.exp - divisor.exp - (t_dividend < t_divisor);

        BigNum cur_div;
        cur_div.digits.erase(cur_div.digits.begin());
        --cur_div.exp;

        for (int i = dividend.digits.size() - 1; i >= 0 || (!cur_div.is_zero() && -i < MAX_ITERATIONS); --i) {
            while (cur_div.exp != cur_div.digits.size() - 1) {
                cur_div.digits.insert(cur_div.digits.begin(), 0);    
            }
            cur_div.digits.insert(cur_div.digits.begin(), i >= 0 ? digits[i] : 0);
            ++cur_div.exp;
            
            int k = 0;
            while (cur_div.digits.back() == 0 && cur_div.digits.size() > 1) {
                cur_div.digits.pop_back();
            }

            while (cur_div > divisor || cur_div == divisor) {
                cur_div = cur_div - divisor;
                ++k;
            }

            res.digits.insert(res.digits.begin(), k);
        }

        while (res.digits.back() == 0) {
            res.digits.erase(res.digits.end() - 1);
        }
        
        res.remove_zeroes();
        return res;
    }

    BigNum& BigNum::operator+=(const BigNum& value) {
        return (*this = *this + value);
    }
    BigNum& BigNum::operator-=(const BigNum& value) {
        return (*this = *this - value);
    }
    BigNum& BigNum::operator*=(const BigNum& value) {
        return (*this = *this * value);
    }
    BigNum& BigNum::operator/=(const BigNum& value) {
        return (*this = *this / value);
    }
    
    BigNum BigNum::pow(int value) const {
        BigNum res = 1_bn;
        BigNum t = *this;
        while (value) {
            if (value % 2) {
                res = res * t;
            }
            t *= t;
            value /= 2;
        }
        return res;
    }

    BigNum BigNum::trunc(int64_t digits_amt) const {
        int64_t frac = digits.size() - exp - 1;
        BigNum res = *this;
        while (frac > digits_amt && frac > 0 && res.digits.size() > 1) {
            res.digits.erase(res.digits.begin());
            --frac;
        }

        if (res.is_zero()) {
            res.sign = 1;
            res.exp = 0;
        }

        return res;
    }

    BigNum calculate_pi(int64_t precision) {
        BigNum arctg1_5 = 0_bn;
        const BigNum C1_5 = 0.2_bn;
        BigNum arctg1_239 = 0_bn;
        const BigNum C1_239 = 1_bn / 239_bn;

        BigNum big_i = 1_bn;
        for (int64_t i = 0; i < precision; ++i, big_i += 2_bn) {
            BigNum cur1 = C1_5.pow(2 * i + 1) / big_i;
            BigNum cur2 = C1_239.pow(2 * i + 1) / big_i;
            if (i % 2) {
                cur1 = -cur1;
                cur2 = -cur2;
            }
            arctg1_5 += cur1;
            arctg1_239 += cur2;
        }

        return (4_bn * (4_bn * arctg1_5 - arctg1_239)).trunc(precision);
    }

    BigNum operator ""_bn(const char* str) {
        return BigNum(str);
    }
} // namespace Bignum