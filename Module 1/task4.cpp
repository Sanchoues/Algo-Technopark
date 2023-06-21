/*Имеется лог-файл, в котором хранятся пары для N пользователей (Идентификатор пользователя, посещаемость сайта).
Напишите программу, которая выбирает K пользователей, которые чаще других заходили на сайт, и выводит их в порядке возрастания 
посещаемости. Количество заходов и идентификаторы пользователей не повторяются.
Требования: время работы O(N * logK), где N - кол-во пользователей. Ограничение на размер кучи O(K).
Формат входных данных: Сначала вводятся N и K, затем пары (Идентификатор пользователя, посещаемость сайта).
Формат выходных данных: Идентификаторы пользователей в порядке возрастания посещаемости
*/
#include <iostream>


template<typename T>
struct DefaultComparator{
    bool operator()(const T& l, const T& r){
        return l < r; 
    }
};

struct Log{
    int user;
    int number;
};

struct LogComparator{
    bool operator()(const Log &l, const Log &r) const{
        return l.number > r.number;
    }
};


template <typename T, typename Comparator = DefaultComparator<T>>
class Heap{
public:
    Heap(Comparator comp): compare(comp), buff(nullptr), buff_size(0),heap_size(0) {
    }

    Heap(const T& arr, size_t arr_size, Comparator comp): compare(comp), buff_size(arr_size), heap_size(arr_size){
        buff = new T[buff_size];
        for (size_t i  = 0; i < arr_size; i++) buff[i] = arr[i];
        for (size_t i = heap_size / 2 - 1; i >= 0; i++) SiftDown(i);
    }

    ~Heap(){
        delete[] buff;
    }

    const T& Top(){
        T& result = buff[0];
        return result;
    }

    T Pop(){
        T result = buff[0];
        buff[0] = buff[--heap_size];
        SiftDown(0);
        return result;
    }

    void Push(const T& value){
        if (buff_size == 0 ) {
            buff_size++;
            buff = new T[buff_size];
        }
        if (heap_size >= buff_size) {
            buff_size = buff_size * 2;
            T *new_buff = new T[buff_size];
            for (size_t i = 0; i < heap_size; i++) new_buff[i] = buff[i];
            delete[] buff;
            buff = new_buff;
        }
        buff[heap_size] = value;
        SiftUp(heap_size);
        heap_size++; 
    }

    bool IsEmpty() const { return heap_size == 0; }
    size_t Size() const { return heap_size; }    

private:
    void SiftUp(size_t index){
        size_t parent;
        while (index > 0){
            parent = (index - 1) / 2;
            if (compare(buff[index],buff[parent])) return;
            std::swap(buff[index], buff[parent]);
            index = parent;
        }
    }
    void SiftDown(size_t index){
        size_t left = 2 * index + 1;
        size_t right = 2 * index + 2;
        size_t least = index;
        if (left < heap_size && compare(buff[index], buff[left])) least = left;
        if (right < heap_size && compare(buff[least], buff[right])) least = right;
        if (least != index){
            std::swap(buff[index], buff[least]);
            SiftDown(least);
        } 
    }

    T* buff;
    Comparator compare;
    size_t buff_size;
    size_t heap_size;
};

void run(int n, int k){
    LogComparator compare;
    Heap<Log, LogComparator> logs(compare);
    Log log;
    for (int i = 0; i < n; i++){
        std::cin >> log.user >> log.number;
        // Ограничиваем размер кучи
        if (logs.Size() < k) logs.Push(log);
        else{
            logs.Push(log);
            logs.Pop();
        } 
    }
    Log result;
    for (int i = 0; i < k; i++){
        result = logs.Pop();
        std::cout << result.user << " ";
    }

}
int main(){
    int n,k;
    std::cin >> n >> k;
    run(n,k);
    return 0;
}
