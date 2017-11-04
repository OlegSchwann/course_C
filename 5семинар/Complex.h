/*Домашнее задание No5
Реализовать класс «Комплексное число» – Complex
В классе перегрузить операции для сложения, вычитания, умножения, деления, присваивания
и вывода на экран. Создать два вектора размерности n из комплексных координат.
Передать их в функцию, которая выполняет сложение комплексных векторов, используя
реализованные операции.*/

#ifndef INC_5_COMPLEX_H
#define INC_5_COMPLEX_H

#include <iostream>
#include <vector>

// класс «Комплексное число»
class Complex{
private:
    float real;
    float complex;
public:
    Complex():real(0),complex(0){}
    Complex(float real, float complex):real(real),complex(complex){}

    ~Complex() = default; //нет динамических полей, не надо ничего освобождать.

    // перегрузить операции: type operator operator-symbol ( parameter-list )
    // сложения friend const Class operator+(const Class& left, const Class& right);
    //Complex Complex::operator+(const Complex &right) const ; - не работает?
    //friend const Complex operator+(const Complex &left, const Complex &right);
    Complex operator+(const Complex &right) const ;

    // вычитания
    friend const Complex operator-(const Complex &left, const Complex &right);

    // умножения
    friend const Complex operator*(const Complex &left, const Complex &right);

    // деления
    friend const Complex operator/(const Complex &left, const Complex &right);

    // присваивания
    Complex& operator=(const Complex &other);
    // вывода на экран
    friend std::ostream &operator<<(std::ostream &os, const Complex &num);
};

std::vector<Complex> vector_addition(const std::vector<Complex> &first, const std::vector<Complex> &second);

#endif //INC_5_COMPLEX_H
