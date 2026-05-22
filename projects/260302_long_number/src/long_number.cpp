#include "long_number.hpp"
#include <cstring>
#include <stdexcept>
#include <string>
#include <algorithm>

using PY::LongNumber;

LongNumber::LongNumber() : numbers(nullptr), length(0), sign(0) {
}

LongNumber::LongNumber(int length, int sign) : length(length), sign(sign) {
    if (length > 0) {
        numbers = new int[length]();
    } else {
        numbers = nullptr;
    }
}

LongNumber::LongNumber(const char* const str) {
    if (!str) {
        numbers = nullptr;
        length = 0;
        sign = 0;
        return;
    }
    
    length = get_length(str);
    sign = 1;
    
    int start = 0;
    if (str[0] == '-') {
        sign = -1;
        start = 1;
        length--;
    }
    
    if (length == 0 || (length == 1 && str[start] == '0')) {
        sign = 0;
        length = 1;
        numbers = new int[1]{0};
        return;
    }
    
    numbers = new int[length];
    for (int i = 0; i < length; i++) {
        numbers[i] = str[start + i] - '0';
    }
}

LongNumber::LongNumber(const LongNumber& x) : length(x.length), sign(x.sign) {
    if (length > 0 && x.numbers) {
        numbers = new int[length];
        for (int i = 0; i < length; i++) {
            numbers[i] = x.numbers[i];
        }
    } else {
        numbers = nullptr;
    }
}

LongNumber::LongNumber(LongNumber&& x) : numbers(x.numbers), length(x.length), sign(x.sign) {
    x.numbers = nullptr;
    x.length = 0;
    x.sign = 0;
}

LongNumber::~LongNumber() {
    delete[] numbers;
}

LongNumber& LongNumber::operator = (const char* const str) {
    delete[] numbers;
    
    if (!str) {
        numbers = nullptr;
        length = 0;
        sign = 0;
        return *this;
    }
    
    length = get_length(str);
    sign = 1;
    
    int start = 0;
    if (str[0] == '-') {
        sign = -1;
        start = 1;
        length--;
    }
    
    if (length == 0 || (length == 1 && str[start] == '0')) {
        sign = 0;
        length = 1;
        numbers = new int[1]{0};
        return *this;
    }
    
    numbers = new int[length];
    for (int i = 0; i < length; i++) {
        numbers[i] = str[start + i] - '0';
    }
    
    return *this;
}

LongNumber& LongNumber::operator = (const LongNumber& x) {
    if (this != &x) {
        delete[] numbers;
        
        length = x.length;
        sign = x.sign;
        
        if (length > 0 && x.numbers) {
            numbers = new int[length];
            for (int i = 0; i < length; i++) {
                numbers[i] = x.numbers[i];
            }
        } else {
            numbers = nullptr;
        }
    }
    return *this;
}

LongNumber& LongNumber::operator = (LongNumber&& x) {
    if (this != &x) {
        delete[] numbers;
        
        numbers = x.numbers;
        length = x.length;
        sign = x.sign;
        
        x.numbers = nullptr;
        x.length = 0;
        x.sign = 0;
    }
    return *this;
}

bool LongNumber::operator == (const LongNumber& x) const {
    if (sign != x.sign) return false;
    if (length != x.length) return false;
    for (int i = 0; i < length; i++) {
        if (numbers[i] != x.numbers[i]) return false;
    }
    return true;
}

bool LongNumber::operator != (const LongNumber& x) const {
    return !(*this == x);
}

bool LongNumber::operator > (const LongNumber& x) const {
    if (sign > x.sign) return true;
    if (sign < x.sign) return false;
    if (sign == 0) return false;
    
    if (length > x.length) return sign == 1;
    if (length < x.length) return sign == -1;
    
    for (int i = 0; i < length; i++) {
        if (numbers[i] > x.numbers[i]) return sign == 1;
        if (numbers[i] < x.numbers[i]) return sign == -1;
    }
    return false;
}

bool LongNumber::operator < (const LongNumber& x) const {
    return x > *this;
}

LongNumber LongNumber::operator + (const LongNumber& x) const {
    if (sign == 0) return x;
    if (x.sign == 0) return *this;
    
    if (sign != x.sign) {
        if (sign == -1) {
            LongNumber temp = *this;
            temp.sign = 1;
            return x - temp;
        } else {
            LongNumber temp = x;
            temp.sign = 1;
            return *this - temp;
        }
    }
    
    int max_len = std::max(length, x.length);
    LongNumber result(max_len + 1, sign);
    
    int carry = 0;
    int i = length - 1;
    int j = x.length - 1;
    int k = result.length - 1;
    
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += numbers[i--];
        if (j >= 0) sum += x.numbers[j--];
        
        result.numbers[k--] = sum % 10;
        carry = sum / 10;
    }
    
    result.remove_leading_zeros();
    return result;
}

LongNumber LongNumber::operator - (const LongNumber& x) const {
    if (x.sign == 0) return *this;
    if (sign == 0) {
        LongNumber result = x;
        result.sign = -x.sign;
        return result;
    }
    
    if (sign != x.sign) {
        LongNumber temp = x;
        temp.sign = -x.sign;
        return *this + temp;
    }
    
    bool is_first_larger = false;
    if (length > x.length) {
        is_first_larger = true;
    } else if (length < x.length) {
        is_first_larger = false;
    } else {
        for (int i = 0; i < length; i++) {
            if (numbers[i] > x.numbers[i]) {
                is_first_larger = true;
                break;
            }
            if (numbers[i] < x.numbers[i]) {
                is_first_larger = false;
                break;
            }
        }
    }
    
    const LongNumber* larger;
    const LongNumber* smaller;
    int result_sign;
    
    if (is_first_larger) {
        larger = this;
        smaller = &x;
        result_sign = sign;
    } else {
        larger = &x;
        smaller = this;
        result_sign = -sign;
    }
    
    LongNumber result(larger->length, result_sign);
    
    int borrow = 0;
    int i = larger->length - 1;
    int j = smaller->length - 1;
    int k = result.length - 1;
    
    while (i >= 0) {
        int diff = larger->numbers[i] - borrow;
        if (j >= 0) diff -= smaller->numbers[j--];
        
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result.numbers[k--] = diff;
        i--;
    }
    
    result.remove_leading_zeros();
    return result;
}

LongNumber LongNumber::operator * (const LongNumber& x) const {
    if (sign == 0 || x.sign == 0) {
        return LongNumber("0");
    }
    
    LongNumber result(length + x.length, sign * x.sign);
    
    for (int i = 0; i < result.length; i++) {
        result.numbers[i] = 0;
    }
    
    for (int i = length - 1; i >= 0; i--) {
        int carry = 0;
        int k = result.length - (length - i);
        
        for (int j = x.length - 1; j >= 0; j--) {
            int product = numbers[i] * x.numbers[j] + carry + result.numbers[k];
            result.numbers[k] = product % 10;
            carry = product / 10;
            k--;
        }
        
        if (carry) {
            result.numbers[k] += carry;
        }
    }
    
    result.remove_leading_zeros();
    return result;
}

LongNumber LongNumber::operator / (const LongNumber& x) const {
    if (x.sign == 0) {
        throw std::runtime_error("Division by zero");
    }
    
    if (sign == 0) {
        return LongNumber("0");
    }
    
    LongNumber a = *this;
    a.sign = 1;
    LongNumber b = x;
    b.sign = 1;
    
    if (a < b) {
        return LongNumber("0");
    }
    
    std::string dividend_str;
    for (int i = 0; i < a.length; i++) {
        dividend_str += std::to_string(a.numbers[i]);
    }
    
    std::string quotient_str;
    LongNumber current("0");
    
    for (char digit_char : dividend_str) {
        std::string current_str;
        if (current.length == 1 && current.numbers[0] == 0 && current.sign == 0) {
            current_str = std::string(1, digit_char);
        } else {
            for (int i = 0; i < current.length; i++) {
                current_str += std::to_string(current.numbers[i]);
            }
            current_str += digit_char;
        }
        current = LongNumber(current_str.c_str());
        
        int count = 0;
        while (!(current < b)) {
            current = current - b;
            count++;
        }
        
        quotient_str += std::to_string(count);
    }
    
    size_t pos = quotient_str.find_first_not_of('0');
    if (pos != std::string::npos) {
        quotient_str = quotient_str.substr(pos);
    } else {
        quotient_str = "0";
    }
    
    LongNumber result(quotient_str.c_str());
    
    LongNumber remainder = a - (result * b);
    
    if (sign == 1 && x.sign == 1) {
        result.sign = 1;
    } else if (sign == -1 && x.sign == -1) {
        result.sign = 1;
        if (!(remainder == LongNumber("0"))) {
            result = result + LongNumber("1");
        }
    } else if (sign == 1 && x.sign == -1) {
        result.sign = -1;
    } else {
        result.sign = -1;
        if (!(remainder == LongNumber("0"))) {
            result = result - LongNumber("1");
        }
    }
    
    result.remove_leading_zeros();
    return result;
}

LongNumber LongNumber::operator % (const LongNumber& x) const {
    if (x.sign == 0) {
        throw std::runtime_error("Division by zero");
    }
    
    if (sign == 0) {
        return LongNumber("0");
    }
    
	// a = b * q + r
    LongNumber quotient = *this / x;  //частное 
    LongNumber product = quotient * x;
    LongNumber remainder = *this - product; // остаток
    
    LongNumber abs_x = x;
    abs_x.sign = 1;
    
    while (remainder.sign == -1) {
        remainder = remainder + abs_x;
    }
    
    while (remainder > abs_x || remainder == abs_x) {
        remainder = remainder - abs_x;
    }
    
    if (remainder.sign == -1) {
        remainder.sign = 1;
    }
    
    remainder.remove_leading_zeros();
    return remainder;
}

int LongNumber::get_length(const char* const str) const noexcept {
    int len = 0;
    while (str && str[len] != '\0') {
        len++;
    }
    return len;
}

void LongNumber::remove_leading_zeros() {
    if (length == 0 || !numbers) return;
    
    int i = 0;
    while (i < length && numbers[i] == 0) {
        i++;
    }
    
    if (i == length) {
        delete[] numbers;
        numbers = new int[1]{0};
        length = 1;
        sign = 0;
        return;
    }
    
    if (i > 0) {
        int new_length = length - i;
        int* new_numbers = new int[new_length];
        
        for (int j = 0; j < new_length; j++) {
            new_numbers[j] = numbers[i + j];
        }
        
        delete[] numbers;
        numbers = new_numbers;
        length = new_length;
    }
}

namespace PY {
    std::ostream& operator << (std::ostream &os, const LongNumber& x) {
        if (x.sign == -1) {
            os << '-';
        }
        
        if (x.numbers) {
            for (int i = 0; i < x.length; i++) {
                os << x.numbers[i];
            }
        } else {
            os << '0';
        }
        
        return os;
    }
}