/*7_2. LSD для long long.
Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше 106. Отсортировать массив методом поразрядной 
сортировки LSD по байтам.*/

#include <iostream>

template<typename T, typename KeyFunction>
void CountSort(T* array, int size, int range, KeyFunction Key){
    //Массив счетчиков
    int *counters_array = new int[range + 1];
    for (int i = 0; i < range  + 1; i++) { 
        counters_array[i] = 0;
    }
    for (int i = 0; i < size; i++) { 
        counters_array[Key(array[i])]++;
    }
    for (int i = 0; i < range; i++) {
        counters_array[i + 1] += counters_array[i]; 
    }
    T *tmp = new T[size];
    int tmp_index = 0;
    for (int index = size - 1; index >= 0; index--) {
        tmp_index = --counters_array[Key(array[index])];
        tmp[tmp_index] = array[index];
    }
    for (int i = 0; i < size; i++ ) {
        array[i] = tmp[i];
    }
    delete[] tmp; 
    delete[] counters_array;
} 

void LSDSort(unsigned long long  *array, int size){
    // 1 байт может принимать 256 значений (2^8)
    int range = 255;
    // Сортируем побайтово массив. В лямбда - функции используем битове операции, чтобы получить нужный байт.
    for (int byte_number = 0; byte_number < 8; byte_number++) {
        CountSort(array, size, range, [byte_number](const unsigned long long value) {
            return ((value & (static_cast<unsigned long long>(255) << 8 * byte_number)) >> 8 * byte_number);
        });
    }
}

void Run(int n) {
    unsigned long long *array = new unsigned long long[n];
    for (int i = 0; i < n; i++) {
        std::cin >> array[i]; 
    }
    LSDSort(array, n);
    for (int i = 0; i < n; i++) {
        std::cout << array[i] << " ";
    }
    delete[] array;
}

int main() {
    int n = 0;
    std::cin >> n;
    Run(n);
    return 0;
}