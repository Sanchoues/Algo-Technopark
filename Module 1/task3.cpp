/*Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
Формат входных данных.
В первой строке количество команд n. n ≤ 1000000.
Каждая команда задаётся как 2 целых числа: a b.
a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back
Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой структуры данных, то ожидается “-1”. 
Формат выходных данных.
Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.

Реализовать очередь с помощью двух стеков.
Требования: Очередь должна быть реализована в виде класса. Стек тоже должен быть реализован в виде класса.
*/

#include <iostream>

class Stack{
public:
    Stack(): top(-1), buffer_size(0), buffer(nullptr){
    }

    Stack(int size): top(-1), buffer_size(size){
        buffer = new int[size];
    }
    ~Stack(){
        delete[] buffer;
    }

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    bool IsEmpty() const { 
        return top == -1; 
    }

    void Push(int value){
        if(top + 1 < buffer_size) buffer[++top] = value;
    }

    int Pop(){
        if(top == -1) return -1; 
        return buffer[top--];
    }

private:
    int top;
    int *buffer;
    int buffer_size; 
};

class Queue{
public:
    Queue(): first_stack(),second_stack() {
    }

    Queue(int size): first_stack(size),second_stack(size) {
    }
    ~Queue(){
        while (!IsEmpty()) Pop();
    }

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    bool IsEmpty() const {
        return first_stack.IsEmpty() && second_stack.IsEmpty();
    };

    void Transfer(){
        while (!first_stack.IsEmpty()){
            second_stack.Push(first_stack.Pop());
        }
    }

    void Push(int value){
        first_stack.Push(value);
    }

    int Pop(){
        if (second_stack.IsEmpty() && !first_stack.IsEmpty()) Transfer();
        return second_stack.Pop();
    }

private:
    Stack first_stack;
    Stack second_stack;
};


int run(std::istream& input, std::ostream& output)
{
    int count = 0;
    int command = 0;
    int value = 0;
    input >> count;
    Queue q(count);
    for (int i = 0; i < count; i++){
        input >> command >> value;
        switch (command){
            case 2: // pop front
                if (q.Pop() != value){
                    output << "NO";
                    return 0;
                } 
                break;
            case 3: // push front
                q.Push(value);
                break;
            default:
                output << "NO";
                return 0;
        }
    }
    output << "YES";
    return 0;   
}


int main(){
    run(std::cin, std::cout);
}