#pragma once

#include <iostream>
#include <cstring>
#include <algorithm>

namespace biv {
    class LongNumber {
    private:
        int* numbers;  // массив цифр числа
        int length;  // длина цифр в чсиле
        int sign; // 1 для положительного, -1 для отрицательного, 0 для нуля

    public:
        LongNumber();
        LongNumber(int length, int sign);
        LongNumber(const char* const str);
        LongNumber(const LongNumber& x);
        LongNumber(LongNumber&& x);

        ~LongNumber();

        LongNumber& operator = (const char* const str);
        LongNumber& operator = (const LongNumber& x);
        LongNumber& operator = (LongNumber&& x);

        bool operator == (const LongNumber& x) const;
        bool operator != (const LongNumber& x) const;
        bool operator > (const LongNumber& x) const;
        bool operator < (const LongNumber& x) const;

        LongNumber operator + (const LongNumber& x) const;
        LongNumber operator - (const LongNumber& x) const;
        LongNumber operator * (const LongNumber& x) const;
        LongNumber operator / (const LongNumber& x) const;
        LongNumber operator % (const LongNumber& x) const;

        bool is_negative() const noexcept;

        friend std::ostream& operator << (std::ostream &os, const LongNumber& x);

    private:
        int get_length(const char* const str) const noexcept;
        void normalize();
        static void add_abs(const LongNumber& a, const LongNumber& b, LongNumber& result);
        static void sub_abs(const LongNumber& a, const LongNumber& b, LongNumber& result);
    };
}
