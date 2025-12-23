#include "io.hpp"

#include <iostream>

using namespace std;

void biv::print_array(const char* const comment, int* arr, const int size) {
 for (int i = 0; i < size; ++i)
  cout << arr[i] << ' ';
 cout << endl;
}