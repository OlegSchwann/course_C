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
#ifndef INC_4_PERSON_H
#define INC_4_PERSON_H


#include <cctype>
#include <string>

class Person {
private:
    //Фамилия
    char *surname;
    //Имя
    char *name;
    //Отчество
    char *middle_name;
    //Адрес
    char *address;
    //Образование
    char *education;
    //Пол
    bool gender; //В 2к17 хранить пол в переменной boolean уже не толерантно...
    //Год рождения.
    __int32_t year;
    //вспомгательная функция - копирует строку, начинающуюся с *from_ptr в созданный массив.
    //возвращает начало новой строки
    char *make_string(char *from_ptr) const;

public:

    //конструкторы
    // конструктор принимает указатели на поля, динамически выделяет память и копирует строки
    Person(char *surname,
        char *name,
        char *middle_name,
        char *address,
        char *education,
        bool gender,
        __int32_t year);

    //конструктор копирования,
    //копирует динамические поля
    Person(const Person *const anybody);

    //деструктор
    //должен освободить все строки!
    ~Person();

    //методы set(...)
    void set_surname(char *surname);
    void set_name(char *name);
    void set_middle_name(char *middle_name);
    void set_address(char *address);
    void set_education(char *education);
    void set_gender(char *gender);
    void set_year(__int32_t);

    //get(...)

    char *const get_surname() const ;
    char *const get_name() const ;
    char *const get_middle_name() const ;
    char *const get_address() const ;
    char *const get_education() const ;
    char get_gender() const ;
    __int32_t get_year() const ;

    //show(...)
    //переопределить вывод <<
    friend std::ostream& operator<<(std::ostream& os, const Person& anybody);
};


#endif //INC_4_PERSON_H
