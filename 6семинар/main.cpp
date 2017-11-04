/* Реализовать базовый класс «Учащийся» и производные классы
 * «Школьник», «Студент» и «Аспирант».
 * Создать массив указателей на объекты базового класса и
 * заполнить этот массив объектами производных классов.
 * Продемонстрировать работу виртуальных методов базового класса при проходе по этому массиву.*/
#include <iostream>

class Learner{ // базовый класс «Учащийся»
public:
    std::string name;
    virtual ~Learner() = default;
    virtual std::string status(){
        return "Learner";
    }
};

class Schoolboy: public Learner{ // производный класс «Школьник»
public:
//    Schoolboy():Learner() {};
    ~Schoolboy() = default;
    std::string status() /*override*/ {
        return "Schoolboy";
    }
};

class Student:Learner{ // производный класс «Студент»
public:
    Student():Learner() {};
    ~Student() = default;
    std::string status() /*override*/ {
        return "Student";
    }
};

class Postgraduate:Learner{ // производный класс «Аспирант»
public:
    Postgraduate():Learner() {};
    ~Postgraduate() = default;
    std::string status() /*override*/ {
        return "Postgraduate";
    }
};


int main() {
    // Создать массив указателей на объекты базового класса
    Learner *learners[4];
    // и заполнить этот массив объектами производных классов.
    learners[0] = new Learner;
    learners[1] = new Schoolboy;
    learners[2] = static_cast<Learner*>(new Student);
    learners[3] = (Learner*)new Postgraduate; //    reinterpret_cast<>

    // Range-based for loop to iterate through the array.
    for(auto i : learners){
        std::cout << i->status() << ' ';
    }
    return 0;
}