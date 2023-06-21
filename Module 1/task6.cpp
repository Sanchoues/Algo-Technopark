/*Дано множество целых чисел из [0..10^9] размера n. 
Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
10%  перцентиль
медиана
90%  перцентиль

Требования: к дополнительной памяти: O(n). 
Среднее время работы: O(n)
Должна быть отдельно выделенная функция partition. 
Рекурсия запрещена. 
Решение должно поддерживать передачу функции сравнения снаружи.


Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении. Описание для случая прохода от начала массива к концу:
Выбирается опорный элемент. Опорный элемент меняется с последним элементом массива.
Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного. Затем располагаются элементы, строго бОльшие опорного. В конце массива лежат нерассмотренные элементы. Последним элементом лежит опорный.
Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j.
Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.

6_1. Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.

*/

#include <iostream>

// Медиана трёх. Выбираем медиану между серединой, началом и концом массива
template<typename T, typename Comparator>
int GetMedian(T *array, int left, int right, Comparator compare){
    int middle = left + (right - left)/2;
    if (compare(array[left], array[middle]) && compare(array[left], array[right])){
        if (compare(array[middle], array[right])) return middle;
        else return right;
    }
    else if (compare(array[middle], array[left]) && compare(array[middle], array[right])){
        if (compare(array[left], array[right])) return left;
        else return right;
    }
    else {
        if (compare(array[middle], array[left])) return middle;
        else return left;
    }
}

template<typename T, typename Comparator>
int Partition(T *array, int left, int right, Comparator compare){
    int pivot_idx = left + (right - left)/2;
    int pivot = array[pivot_idx];
    int pivor_idx = GetMedian(array, left, right, compare);

    std::swap(array[pivot_idx], array[right - 1]);

    //  i - индекс начала группы элементов, меньше опорного. j - индекс начала группы элементов, больше опорного
    int i = left;
    int j = i;
    while(j < right - 1){
        while(compare(array[i], pivot)){
            i++;
        }
        if (j <= i ) j = i;
        // array[i] >= pivot
        while(!compare(array[j], pivot) && j < right - 1){
            j++;
        }
        // array[i] >= pivot && array[j] < pivot
        if (!compare(array[i], pivot) && compare(array[j], pivot) && j < right - 1){
            std::swap(array[i], array[j]);
            i++;
            j++;
        }
    }
    std::swap(array[i],array[right - 1]);
    return i;
}

template<typename T, typename Comparator>
void KthElement(T *array, int size, int k, Comparator compare){
    int left = 0;
    int right = size;
    int result;
    while (left < right) {
        result = Partition(array, left, right, compare);
        if (result == k){
            return;
        }
        else if (result < k){
            left = result + 1;
        }
        else {
            right = result;
        }
    }
}

void Solve(int n){
    int *array = new int[n];
    for (int i = 0; i < n; i++) std::cin >> array[i];
    auto is_less = [](const int& l, const int& r){
        return l < r;
    };

    int percentile_ten = n/10;
    KthElement(array, n, percentile_ten, is_less);
    std::cout << array[percentile_ten] << std::endl;

    int median_index = n/2;
    KthElement(array, n, median_index, is_less);
    std::cout << array[median_index] << std::endl;   
    int percentile_ninety = 90 * n/100;
    KthElement(array, n, percentile_ninety, is_less); 
    std::cout << array[percentile_ninety] << std::endl;

    delete[] array;
}

int main(){
    int n;
    std::cin >> n;
    Solve(n);
    return 0;
}