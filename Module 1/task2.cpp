/*2_4. Дан отсортированный массив различных целых чисел A[0..n-1] и массив целых чисел B[0..m-1]. 
Для каждого элемента массива B[i] найдите минимальный индекс элемента массива A[k], ближайшего по значению к B[i].
Требования: Время работы поиска для каждого элемента B[i]: O(log(k)). 
Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска размером порядка k 
с помощью экспоненциального поиска, а потом уже в нем делать бинарный поиск.
n ≤ 110000, m ≤ 1000.
*/

#include <iostream>

void Search(int *first_array, int first_size, int *second_array, int second_size, int *result);
int ExponentialSearch(int *array, int array_size, int value);
int BinarySearch(int *array, int value, int first, int last);


int main(){
    int first_size = 0;
    int second_size = 0;

    std::cin >> first_size;
    int * first_array = new int[first_size]; 
    for (int i = 0; i< first_size; i++ ) std::cin >> first_array[i];
    std::cin >> second_size;

    int * second_array = new int[second_size]; 
    for (int i = 0; i< second_size; i++ ) std::cin >> second_array[i];

    int *result = new int[second_size];
    Search(first_array, first_size, second_array, second_size, result);
    for (int i = 0; i < second_size; i++) std::cout<<result[i]<<' '; 

    delete [] first_array, second_array, result;
    return 0;
}

void Search(int *first_array, int first_size, int *second_array, int second_size, int *result){
    int left = 0;
    int right = 2;
    int index = 0;
    for (int i = 0; i < second_size; i++){
        /*При помощи экспоненциального поиска получаем правую границу и вычисляем левую.*/
        right = ExponentialSearch(first_array, first_size, second_array[i]);
        if (right != 2) left = left >> 1;
        else left = 0;
        index = BinarySearch(first_array, second_array[i], left, right);
        /*Бинарный поиск даст нам позицию, где должен стоять элемент, поэтому, если элемент не стоит на границе массива, 
        мы сраниваем с соседними элементами */
        if (index != 0 && second_array[i] < first_array[first_size -1]){
            if (second_array[i] - first_array[index-1] <= first_array[index] - second_array[i]) result[i] = index - 1;
            else result[i] = index;
        }
        else result[i] = index;
    }
}

int ExponentialSearch(int *array, int array_size, int value){
    int local_right = 2;
    int local_left = 0;
    while(array[local_right] < value){
        local_left = local_right;
        local_right *= 2;
        if (local_right >= array_size) {
            local_right = array_size - 1;
            break;
        }
    }
    return local_right;
}

int BinarySearch(int *array, int value, int first, int last){
    int middle = 0;
    while(first < last){
        middle = (first + last)/2;
        if (array[middle] < value )  first = middle + 1;
        else last = middle;
    }
    return first;
}