/*Домашнее задание №7
. Написать несколько конструкторов, в том числе конструктор копирования. Элементы очереди хранятся в массиве. Если массив имеет фиксированную размерность, то предусмотреть контроль выхода за пределы массива. Если память выделяется динамически и ее не хватает, то увеличить размер выделенной памяти. В классе реализовать методы добавления элементов в очередь (в начала и в конец очереди) и удаление элементов из очереди (из начала и из конца очереди). Перегрузить операции сложения, вычитания,присваивания и вывода на экран. Создать массив объектов. Передавать объекты в функцию, которая удаляет из очереди первый (с головы очереди), третий, пятый и т. д. (нечётные) элементы. Реализовать предложенный шаблонный класс (тип элементов, которые хранит класс – параметр шаблона; каждый объект класса может хранить элементы только одного типа). Продемонстрировать реализованную функциональность класса для работы с различными типами данных. Исключительные ситуации необходимо обрабатывать через механизм исключений.*/

#include <iostream>
#include <cstring>
#include "Deque.hpp"

//Передавать объекты в функцию, которая удаляет из очереди первый (с головы очереди), третий, пятый и т. д. (нечётные) элементы.
//template<template <typename Stored> class Container> - неполучается шаблонизировать
template<typename Stored>
Deque<Stored> odd_elements(const Deque<Stored> &container){
    bool is_odd = true;
    Deque<Stored> out_container;
    for(const Stored i : container){
        if(is_odd){
            out_container.push_back(i);
        };
        is_odd = !is_odd;
    }
    return out_container;
};

int main() {
    //«Очередь c двумя концами» – Deque
    Deque<char> *deque =  new Deque<char>(5);
    //В классе реализовать методы добавления элементов в очередь (в начала и в конец очереди) и удаление элементов из очереди (из начала и из конца очереди)
    char a = 'a';
    deque->push_back(a);
    //std::cout << "1:" << *deque << std::endl;
    char b = 'b';
    deque->push_back(b);
    //std::cout << "2:" << *deque << std::endl;
    char c = 'c';
    deque->push_back(c);
    //std::cout << "3:" << *deque << std::endl;

    char d = 'd';
    deque->push_front(d);
    //std::cout << "4:" << *deque << std::endl;
    char e = 'e';
    deque->push_front(e);
    //std::cout << "5:" << *deque << std::endl;
    char f = 'f';
    deque->push_front(f);
    //std::cout << "6:" << *deque << std::endl;

    std::cout << "старая очередь: " << *deque << std::endl;
    //Написать несколько конструкторов, в том числе конструктор копирования
    Deque<char> new_deque = *deque;
    std::cout << "скопированная очередь: " << new_deque << std::endl;

    new_deque += *deque;
    std::cout << "конкатенация очередей: " << new_deque << std::endl;

    Deque<char> sum_deque = (*deque + new_deque);
    std::cout << "сумма очередей: " << (*deque + new_deque) << std::endl;

    c = deque->pop_back();
    b = deque->pop_back();
    f = deque->pop_front();
    e = deque->pop_front();
    d = deque->pop_front();
    a = deque->pop_front();
    std::cout << a << b << c << d << e << f << std::endl;

    *deque = new_deque;
    std::cout << "скопировано в уже существующий объект: " << *deque << std::endl;

    std::cout << "только нечётные элементы: " << odd_elements<char>(new_deque) << std::endl;

    //Создать массив объектов:
    Deque<char> array[3];
    array[0] = *deque;
    array[1] = new_deque;
    array[2] = sum_deque;


    //Продемонстрировать реализованную функциональность класса для работы с различными типами данных
    Deque<int> *int_deque =  new Deque<int>();
    int_deque->push_back(35);
    int_deque->push_front(25);
    std::cout << "2 числа: " << *int_deque << std::endl;
    return 0;
}