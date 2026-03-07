#include "long_number.hpp"
#include <cstring>

using biv::LongNumber;
  
// пустой конструктор
LongNumber::LongNumber() : numbers(nullptr), length(0), sign(0) {
 // создаёт "пустое число", у которого нет выделенной памяти и length = 0, sign = 0
}

// констурктор с длиной и знаком
LongNumber::LongNumber(int length, int sign) : length(length), sign(sign) {
 // выделяет память под указанное кол-во цифр
 numbers = new int[length]();
}

// констуртор из строки
LongNumber::LongNumber(const char* const str) {
 length = get_length(str); // длина строки
 //опрределяем знак
 sign = 1;
 
 if (str[0] == '-') {
  sign = -1;
  length--;
 }
 
 if (length == 1 && str[0] == '0') {
  sign = 0;
 }
 
 numbers = new int[length]; //выделяем память
 
 int num = (str[0] == '-') ? 1 : 0; // с какого символа начинаются цифры
 for (int i = 0; i < length; i++) {
  numbers[i] = str[num + i] - '0';
 }
}
// конструктор копирования
LongNumber::LongNumber(const LongNumber& x) : length(x.length), sign(x.sign) {
 numbers = new int[length];  // выделяем новую память
 // копируем все цифры
 for (int i = 0; i < length; i++) {
  numbers[i] = x.numbers[i];
 }
}
// конструктор перемещения
LongNumber::LongNumber(LongNumber&& x) : numbers(x.numbers), length(x.length), sign(x.sign) {
 x.numbers = nullptr;
 x.length = 0;
 x.sign = 0;
}

// деструтор
LongNumber::~LongNumber() {
 delete[] numbers; // освобождает выделенную память
}

// оператор присваивания из строки
// принимает указатель на константную строку
LongNumber& LongNumber::operator = (const char* const str) {
 delete[] numbers; // удаляем старые данные
 
 length = get_length(str);
 sign = 1;
 
 if (str[0] == '-') {
  sign = -1;
  length--;
 }
 
 if (length == 1 && str[0] == '0') {
  sign = 0;
 }
 
 numbers = new int[length];
 
 int start = (str[0] == '-') ? 1 : 0;
 for (int i = 0; i < length; i++) {
  numbers[i] = str[start + i] - '0';
 }
 
 return *this;  // возвращаем ссылку на себя для цепочек вида a = b = c
}

// копирующее присваивание
// принимает ссылку на другой объект
LongNumber& LongNumber::operator = (const LongNumber& x) {
 if (this != &x) { // чтобы не было a = a
  delete[] numbers;
  
  length = x.length;
  sign = x.sign;
  numbers = new int[length];
  
  for (int i = 0; i < length; i++) {
   numbers[i] = x.numbers[i];
  }
 }
 
 return *this;
}

// перемещающее присваивание
LongNumber& LongNumber::operator = (LongNumber&& x) {
 if (this != &x) {
  delete[] numbers;
  // забираем знак длину и указатель
  numbers = x.numbers; 
  length = x.length;
  sign = x.sign;
  // обнуляем все для x
  x.numbers = nullptr;
  x.length = 0;
  x.sign = 0;
 }
 
 return *this;
}

bool LongNumber::operator == (const LongNumber& x) const {
 if (sign != x.sign) return false; // сравниваем знаки
 if (length != x.length) return false; // сравниваем длину
 // сравниваем цифры
 for (int i = 0; i < length; i++) {
  if (numbers[i] != x.numbers[i]) return false;
 }
 
 return true;
}

bool LongNumber::operator != (const LongNumber& x) const {
 return !(*this == x);
}

bool LongNumber::operator > (const LongNumber& x) const {
 if (sign > x.sign) return true; // + > -
 if (sign < x.sign) return false; // - < +
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
 if (sign == 0) return x; // 0 + x = x
 if (x.sign == 0) return *this; // x + 0 = x
 
 // если одинаковые знаки
 if (sign == x.sign) {
  LongNumber result(std::max(length, x.length) + 1, sign);
  
  int carry = 0; // перенос
  int i = length - 1; // идём с конца
  int j = x.length - 1;
  int k = result.length - 1;
  
  while (i >= 0  j >= 0  carry) {
   int sum = carry;
   if (i >= 0) sum += numbers[i--];
   if (j >= 0) sum += x.numbers[j--];
   
   result.numbers[k--] = sum % 10; // текущая цифра
   carry = sum / 10; // перенос
  }
  
  result.remove_leading_zeros();
  return result;
 }
 
 // если разные знаки, превращаем в вычитание
 if (sign == -1 && x.sign == 1) {
  LongNumber temp = *this;
  temp.sign = 1;
  return x - temp;
 } 
 else {
  LongNumber temp = x;
  temp.sign = 1;
  return *this - temp;
 }
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
 
 bool abs_compare = false;
 if (length > x.length) abs_compare = true;
 else if (length < x.length) abs_compare = false;
 else {
  for (int i = 0; i < length; i++) {
   if (numbers[i] > x.numbers[i]) {
    abs_compare = true;
    break;
   }
   if (numbers[i] < x.numbers[i]) {
    abs_compare = false;
    break;
   }
  }
 }
 
 if (abs_compare) {
  LongNumber result(length, sign);
  
  int borrow = 0;
  int i = length - 1;
  int j = x.length - 1;
  int k = result.length - 1;
  
  while (i >= 0) {
   int diff = numbers[i] - borrow;
   if (j >= 0) diff -= x.numbers[j--];
   
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
 } else {
  LongNumber result = x - *this;
  result.sign = -sign;
  return result;
 }
}

LongNumber LongNumber::operator * (const LongNumber& x) const {
 if (sign == 0 || x.sign == 0) {
  return LongNumber("0");
 }
 
 LongNumber result(length + x.length, sign * x.sign);
 
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
 
 LongNumber dividend = *this;
 dividend.sign = 1;
 
 LongNumber divisor = x;
 divisor.sign = 1;
 
 if (dividend < divisor) {
  return LongNumber("0");
 }
 
 LongNumber quotient(std::max(1, dividend.length - divisor.length + 1), sign * x.sign);
 LongNumber remainder("0");
 
 for (int i = 0; i < dividend.length; i++) {
  remainder = remainder * LongNumber("10") + LongNumber(std::to_string(dividend.numbers[i]).c_str());
  
  int count = 0;
  while (!(remainder < divisor)) {
   remainder = remainder - divisor;
   count++;
  }
  
  if (i >= quotient.length) {
   // Добавляем новую цифру в начало
   LongNumber temp(quotient.length + 1, quotient.sign);
   temp.numbers[0] = count;
   for (int j = 0; j < quotient.length; j++) {
    temp.numbers[j + 1] = quotient.numbers[j];
   }
   quotient = std::move(temp);
  } else {
   quotient.numbers[quotient.length - dividend.length + i] = count;
  }
 }
 
 quotient.remove_leading_zeros();
 return quotient;
}

LongNumber LongNumber::operator % (const LongNumber& x) const {
 if (x.sign == 0) {
  throw std::runtime_error("Division by zero");
 }
 
 LongNumber quotient = *this / x;
 LongNumber product = quotient * x;
 return *this - product;
}

bool LongNumber::is_negative() const noexcept {
 return sign == -1;
}

// ----------------------------------------------------------
// PRIVATE
// ----------------------------------------------------------
int LongNumber::get_length(const char* const str) const noexcept {
 int len = 0;
 while (str[len] != '\0') {
  len++;
 }
 return len;
}

void LongNumber::remove_leading_zeros() {
 int i = 0;
 while (i < length && numbers[i] == 0) {
  i++;
 }
 
 if (i == length) {
  length = 1;
  delete[] numbers;
  numbers = new int[1]{0};
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

namespace biv {
 std::ostream& operator << (std::ostream &os, const LongNumber& x) {
  if (x.sign == -1) {
   os << '-';
  }
  
  for (int i = 0; i < x.length; i++) {
   os << x.numbers[i];
  }
  
  return os;
 }
}