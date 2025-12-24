#include <iostream>

void merge(int arr[], int left, int mid, int right);
void merge_sort(int arr[], int size);
void merge_sort_recursive(int arr[], int left, int right);

int main() {
    int size;
    
    std::cout << "Введите количество элементов: ";
    std::cin >> size;
    
    int* arr = new int[size];
    
    std::cout << "Введите " << size << " элементов:" << std::endl;
    for (int i = 0; i < size; ++i) {
        std::cin >> arr[i];
    }
    
    std::cout << std::endl << "Исходный массив: ";
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    
    merge_sort(arr, size);
    
    std::cout << "Отсортированный массив: ";
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    
    delete[] arr;
    return 0;
}

void merge(int arr[], const int left, const int mid, const int right) {
    const int size = right - left + 1;
    int* temp = new int[size];
        
    int i = left; 
    int j = mid + 1; 
    int k = 0;
    
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i];
            i++;
            k++;
        } else {
            temp[k] = arr[j];
            j++;
            k++;
        }
    }
    
    while (i <= mid) {
        temp[k] = arr[i];
        i++;
        k++;
    }

    while (j <= right) {
        temp[k] = arr[j];
        j++;
        k++;
    }
    
    for (int i = 0; i < k; i++) {
        arr[left + i] = temp[i];
    }
    
    delete[] temp;
}

void merge_sort(int arr[], const int size) {
    if (size == 0) {
        return;
    }

    merge_sort_recursive(arr, 0, size - 1);
}

void merge_sort_recursive(int arr[], const int left, const int right) {
    if (left >= right) {
        return;
    }

    const int mid = left + (right - left) / 2;

    merge_sort_recursive(arr, left, mid);
    merge_sort_recursive(arr, mid + 1, right);

    merge(arr, left, mid, right);
}
