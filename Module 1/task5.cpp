/*Во всех задачах данного раздела необходимо реализовать и использовать сортировку слиянием в виде шаблонной функции. 
Решение должно поддерживать передачу функции сравнения снаружи.
Общее время работы алгоритма O(n log n).

5_2. Современники.
Группа людей называется современниками если был такой момент, когда они могли собраться вместе. Для этого в этот момент каждому из них должно было  уже исполниться 18 лет, но ещё не исполниться 80 лет.
Дан список Жизни Великих Людей. Необходимо получить максимальное количество современников. В день 18летия человек уже может принимать участие в собраниях, а в день 80летия и в день смерти уже не может.
Замечание. Человек мог не дожить до 18-летия, либо умереть в день 18-летия. В этих случаях принимать участие в собраниях он не мог.
*/
#include <iostream>

struct Time {
    int day;
    int month;
    int year; 
};

struct TimeComparator {
    bool operator()(const Time &l, const Time &r) const {
        if (l.year == r.year && l.month == r.month) { 
            return l.day < r.day;
        }
        else if (l.year == r.year) {
            return l.month < r.month;
        }
        else {
            return l.year < r.year;
        }
    }
};

struct Person {
    Time data;
    // True - живой , False - мертвый
    bool status;
};

struct PersonComparator {
    TimeComparator time_compare;
    bool operator()(const Person &l, const Person &r){
        // Если даты совпадают, то сортируем по статусу. Сначала false.
        if (l.data.year == r.data.year && l.data.month == r.data.month && l.data.day == r.data.day) {
            return r.status;
        }
        else {
            return time_compare(l.data, r.data);
        }
    }
};

template<typename T>
struct DefaultComparator {
    bool operator()(const T& l, const T& r){
        return l < r; 
    }
};



template <typename T, typename Comparator = DefaultComparator<T>>
void Merge(T* array,size_t left, size_t middle, size_t end, Comparator compare = DefaultComparator<T>()) {
    T* buff = new T[end - left + 1];
    size_t left_index = left;
    size_t right_index = middle + 1;
    size_t buff_index = 0;
    for ( ; left_index <= middle && right_index <= end; buff_index++) {
        if (compare(array[left_index], array[right_index])) {
            buff[buff_index] = array[left_index++];    
        }
        else {
            buff[buff_index] = array[right_index++];  
        }
    }
    while (left_index <= middle) {
        buff[buff_index++] = array[left_index++];
    }
    while (right_index <= end) {
        buff[buff_index++] = array[right_index++];
    }
    for (size_t i = left; i <= end; i++) {
        array[i] = buff[i-left];
    }
    delete[] buff;
}

template <typename T, typename Comparator = DefaultComparator<T>>
void MergeSort(T* array,size_t begin, size_t end, Comparator compare = DefaultComparator<T>()) {
    if (end <= begin) return;
    size_t middle = (end - begin)/2 + begin;
    MergeSort(array, begin, middle, compare);
    MergeSort(array, middle + 1, end,compare);
    Merge(array, begin, middle, end, compare);
}


void Run(int n) {
    Person *array = new Person[n * 2];
    size_t length = 0;
    Time birth, death;
    Person human;
    for (size_t i = 0; i < n; i++) {
        std::cin >> birth.day >> birth.month >> birth.year;
        std::cin >> death.day >> death.month >> death.year;
        // Если человек умер до 18 летия, то мы его не записываем.
        if (birth.year + 18 > death.year) {
            continue;
        }
        if (birth.year + 18 == death.year && (birth.month > death.month || 
           (birth.month == death.month && birth.day >= death.day))) {
            continue;
        }
        // Проверяем, что возраст человека не более 80 лет
        if (death.year - birth.year > 80 || (death.year - birth.year == 80 && 
            (death.month > birth.month || (death.month == birth.month && death.day > birth.day)))) { 
            death.year = birth.year + 80;
            death.month = birth.month;
            death.day = birth.day; 
        }
        birth.year += 18;
        human.data = birth;
        human.status = true;
        array[length++] = human;
        human.data = death;
        human.status = false;
        array[length++] = human;        
    }
    PersonComparator compare;
    if (length > 1) MergeSort(array, 0,length - 1, compare);
    int count = 0;
    int count_max = 0;
    for (size_t i = 0; i < length; i++) {
        if (array[i].status){
            count++;    
        } 
        else {
            count--;
        }
        if (count > count_max) {
           count_max = count; 
        }
    }
    std::cout << count_max;
    delete[] array;
}


int main() {
    int n;
    std::cin >> n;
    Run(n);
    return 0;
}
