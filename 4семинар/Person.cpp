
#include "Person.h"
#include <iostream>
#include <cstring>

char * Person::make_string(char *from_ptr) const {
    //вспомогательное -
    //находит длинну строки, начинающуюся с from_ptr
    //выделяет массив сhar такой длинны+1
    //копирует строку в эту память
    size_t sring_len = strlen(from_ptr)+1;
    char *to_ptr = new char[sring_len];
    // Copy N bytes of SRC to DEST.
    memcpy(to_ptr, from_ptr, sring_len);
    return to_ptr;
}

//конструкторы,
Person::Person(char *surname,
               char *name,
               char *middle_name,
               char *address,
               char *education,
               bool gender,
               __int32_t year){
    this->surname = make_string(surname);
    this->name = make_string(name);
    this->middle_name = make_string(middle_name);
    this->address = make_string(address);
    this->education = make_string(education);
    this->gender = gender;
    this->year = year;
    //а если передали бред, надо вызвать деструктор?
}

//конструктор копирования,
//забавно, что он может достучатся до приватных полей переданного ему экземпляра класса,
// нет защиты полей между экземплярами, только между разными классами.
Person::Person(const Person * const anybody){
    this->surname = make_string(anybody->surname);
    this->name = make_string(anybody->name);
    this->middle_name = make_string(anybody->middle_name);
    this->address = make_string(anybody->address);
    this->education = anybody->education;
    this->gender = anybody->gender;
    this->year = anybody->year;
}

//деструктор
Person::~Person(){
//надо вызывать деструкторы статических полей? - нет, даже классов
    delete[](this->surname);
    //Имя
    delete[](this->name);
    //Отчество
    delete[](this->middle_name);
    //Адрес
    delete[](this->address);
    //Образование
    delete[](this->education);
}

//методы set(...)
void Person::set_surname(char *const surname){
    this->surname = surname;
}

void Person::set_name(char *const name){
    this->name = make_string(name);
}

void Person::set_middle_name(char *const middle_name){
    this->middle_name = make_string(middle_name);
}

void Person::set_address(char *const address){
    this->address = make_string(address);
}

void Person::set_education(char *const education){
    this->education = make_string(education);
}

void Person::set_gender(char *const gender){
    this->gender = (*gender == 'M' || *gender == 'm');
}

void Person::set_year(__int32_t year){
    this->year = year;
}

//get(...)
//возвращает указатель на сишную строку в куче
char *const  Person::get_surname() const {
    return make_string(this->surname);
}

char *const  Person::get_name() const {
    return make_string(this->name);
}

char *const  Person::get_middle_name() const {
    return make_string(this->middle_name);
}

char *const  Person::get_address() const {
    return make_string(this->address);
}

char *const  Person::get_education() const {
    return make_string(this->education);
}

char Person::get_gender() const {
    return ( this->gender? 'M': 'F');
}

__int32_t Person::get_year() const {
    return this->year;
}


//show(...).
//Перегрузка операции вывода в поток
std::ostream& operator<<(std::ostream& os, const Person& anybody) {
    os << anybody.surname << "\n" <<
          anybody.name << "\n";
    if(anybody.middle_name && anybody.middle_name[0]){
        os << anybody.middle_name << "\n";
    }
    os << anybody.address << "\n" <<
          anybody.education << "\n" <<
          anybody.get_gender() << "\n" <<
          anybody.year << "\n";
    return os;
}
