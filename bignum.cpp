#include "bignum.hpp"
#include <string.h>
#define MAX_ITERATIONS 100

Bignum::Bignum() {
    exp = 0;
    sign = 1;
    digits = std::vector<int8_t>(1, 0);
}

Bignum::Bignum(const char* str) {
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

void Bignum::remove_zeroes() {
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

std::string Bignum::to_string() {
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

bool Bignum::is_zero() const {
    return digits.size() == 1 && 
           digits[0] == 0;
}

Bignum Bignum::operator-() const {
    Bignum res;
    res.exp = exp;
    res.digits = digits;
    res.sign = -sign;
    
    return res;
}

bool Bignum::operator<(const Bignum& value) const {
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

bool Bignum::operator==(const Bignum& value) const {
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

bool Bignum::operator>(const Bignum& value) const {
    return !(*this < value || *this == value);
}

bool Bignum::operator!=(const Bignum& value) const {
    return !(*this == value);
}

Bignum Bignum::operator+(const Bignum& value) const {
    if (sign > value.sign) {
        return *this - (-value);
    }

    if (sign < value.sign) {
        return value - (-(*this));
    }

    Bignum res;
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

Bignum Bignum::operator-(const Bignum& value) const {
    if (sign != value.sign) {
        return *this + (-value);
    }

    if (sign == -1 && value.sign == -1) {
        return -value - -(*this);
    }

    bool cmp = *this > value;
    const Bignum& tmp1 = cmp ? *this : value;
    const Bignum& tmp2 = cmp ? value : *this;

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

    Bignum res;
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

Bignum Bignum::operator*(const Bignum& value) const {
    Bignum res;
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

Bignum Bignum::operator/(const Bignum& value) const {
    Bignum res;
    Bignum dividend(*this);
    Bignum divisor(value);
    res.sign = dividend.sign * divisor.sign;
    dividend.exp += divisor.digits.size() - divisor.exp - 1;
    divisor.exp = divisor.digits.size() - 1;
    
    res.exp = dividend.exp - divisor.exp - 2;

    Bignum cur_div;
    cur_div.digits.erase(cur_div.digits.begin());
    --cur_div.exp;
    bool div_init_flag = false;
    for (int i = dividend.digits.size() - 1; i >= 0 || (!cur_div.is_zero() && -i < MAX_ITERATIONS); --i) {
        cur_div.digits.insert(cur_div.digits.begin(), i >= 0 ? digits[i] : 0);
        ++cur_div.exp;
        int k = 0;
        while (cur_div > divisor || cur_div == divisor) {
            div_init_flag = true;
            cur_div = cur_div - divisor;
            ++k;
        }
        res.digits.insert(res.digits.begin(), k);
        res.exp += div_init_flag && (i >= 0);
    }

    while (res.digits.back() == 0) {
        res.digits.erase(res.digits.end() - 1);
    }
    
    res.remove_zeroes();
    return res;
}

Bignum operator ""_bn(const char* str) {
    return Bignum(str);
} 