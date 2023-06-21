/*1.2 Вернуть значение бита в числе N по его номеру K.
Формат входных данных. Число N, номер бита K*/
#include <iostream>

int BitValue(int value, int number){
    return (value >> number) & 1;
}

int main(){
    unsigned long long value = 0;
    unsigned int number = 0;
    std::cin >> value >> number; 
    std::cout << BitValue(value, number); 
}