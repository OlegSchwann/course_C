#include "Complex.h"
// класс «Комплексное число»

// перегрузить операции: type operator operator-symbol ( parameter-list )
// сложения friend const Class operator+(const Class& left, const Class& right);
Complex Complex::operator+(const Complex &right) const {
    return Complex(real + right.real, complex + right.complex);
};

// вычитания
const Complex operator-(const Complex &left, const Complex &right) {
    return Complex(left.real - right.real, left.complex - right.complex);
};

// умножения
const Complex operator*(const Complex &left, const Complex &right) {
    //z₁ = a₁ + b₁i, z₂ = a₂ + b₂i
    //z₃ = z₁ * z₂ = (a₁a₂ - b₁b₂) + (a₁b₂ + b₁a₂)i
    return Complex(left.real*right.real - left.complex*right.complex,
                   left.real*right.complex + left.complex*right.real);
}

// деления
const Complex operator/(const Complex &left, const Complex &right) {
    //z₁ = a₁ + b₁i, z₂ = a₂ + b₂i
    //z₃ = z₁ / z₂ = (a₁ + b₁i) / (a₂ + b₂i) =
    // = ((a₁a₂ + b₁b₂) / (a₂² + b₂²)) + ((b₁a₂ - a₁b₂) / (a₁² + b₂²))i
    return Complex(
            (left.real*right.real + left.complex*right.complex) / (right.real*right.real + right.complex*right.complex),
            (left.complex*right.real - left.real*right.complex) / (right.real*right.real + left.complex*left.complex));
}

// присваивания
Complex& Complex::operator=(const Complex &other){
    this->real = other.real;
    this->complex = other.complex;
    return *this;
}

// вывода на экран
std::ostream &operator<<(std::ostream &os, const Complex &num){
    if (num.complex < 0){
        os << num.real << "-" << -num.complex << "i";
    } else {
        os << num.real << "+" << num.complex << "i";
    }
    return os;
}

// Создать два вектора размерности n из комплексных координат. Передать их в функцию, которая выполняет
// сложение комплексных векторов, используя реализованные операции.
std::vector<Complex> vector_addition(std::vector<Complex> *const first, std::vector<Complex> *const second){
    std::vector<Complex> result;
    if (first->size() == second->size()){
        for (int i = 0; i < first->size(); i++){
            result.push_back((*first)[i] + (*second)[i]);
        }
    } else{

    }
    return result;
}