/*Разработать классы для описанных ниже объектов. Включить в класс конструкторы, конструктор
копирования, деструктор, методы set(...), get(...), show(...). Память под строковые поля
необходимо выделять динамически. Определить другие необходимые методы.
Person:
– Фамилия
– Имя
– Отчество
– Адрес
– Пол
– Образование
– Год рождения.
Создать массив объектов. Вывести:
1. список граждан, возраст которых превышает заданный;
2. список граждан с высшим образованием;
3. список граждан мужского пола.
*/
#include <iostream>
#include "Person.h"


int main() {
    Person *one = new Person("Шванн", "Олег", "", "Москва", "2 курс МГТУ", "M", 1998);
    Person *other = new Person(one);
    std::cout << "Первый класс:\n" << *one <<
                 "Второй класс:\n" << *other << std::endl;
    delete one;
    delete other;
    return 0;
}