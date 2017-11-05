/*Домашнее задание №7
«Очередь c двумя концами» – Deque. Написать несколько конструкторов, в том числе конструктор копирования. Элементы очереди хранятся в массиве. Если массив имеет фиксированную размерность, то предусмотреть контроль выхода за пределы массива. Если память выделяется динамически и ее не хватает, то увеличить размер выделенной памяти. В классе реализовать методы добавления элементов в очередь (в начала и в конец очереди) и удаление элементов из очереди (из начала и из конца очереди). Перегрузить операции сложения, вычитания,присваивания и вывода на экран. Создать массив объектов. Передавать объекты в функцию, которая удаляет из очереди первый (с головы очереди), третий, пятый и т. д. (нечётные) элементы. Реализовать предложенный шаблонный класс (тип элементов, которые хранит класс – параметр шаблона; каждый объект класса может хранить элементы только одного типа). Продемонстрировать реализованную функциональность класса для работы с различными типами данных. Исключительные ситуации необходимо обрабатывать через механизм исключений.*/

#ifndef INC_7_DEQUE_H
#define INC_7_DEQUE_H

#include <stdexcept>
#include <cstring>

/* основное, что я хочу:
 * кольцевой буфер, с указателями на предыдущий перед первым и на следующий после последнего
 * элемент в буфере - то есть те места, куда запишется значение при вызове операции добавления
 * если они оказываются равными, то выделяется массив в два раза больше,
 * копируется первая часть от начала до , пустота, вторая часть.
 * Как могут располагатся данные в памяти?
 * ==>-----<====  [≡]>  ==≡>----<====
 * --<====>-----  <[≡]  -<≡===>-----
 * основная сложность - краевые стучаи
 * без увеличеня массива
 * если указатель равен началу массива, то пишем в эту точку и переносим указатель в конец
 *                     (концу)                                                      (начало)
 * и потом стандартная проверка - не равны ли указатели, если да - то увеличиваем массив
 * <=====>------  <[≡]  ≡=====>-----<
 * с увеличением
 * <===========>  <[≡]  ≡===========☓  ≡===========>------------<
 * <===========>  [≡]>  ☓===========≡  >------------<===========≡
 * =====><======  [≡]>  =====≡☓======  =====≡>------------<======
 */

//шаблон. Это значит, что все функции с недоопределёнными типами надо реализовать в header.
template < typename Stored>
class Deque {
public:
    // конструкторы:
    Deque(int InitialCapacity = 1024){ //просто создаёт пустой массив, устанавливает все указатели
        beginning_of_buffer = new Stored[InitialCapacity];
        end_of_buffer = beginning_of_buffer + InitialCapacity - 1;
        head_of_ring = beginning_of_buffer + InitialCapacity/2;
        tail_of_ring = beginning_of_buffer + InitialCapacity/2 +1;
        // -----<>-----
    };

    //конструктор копирования
    //важно скопировать не только массив, но и вызвать конструкторы копирования для каждого
    //объекта в этом массиве. Но для формируемого массива elongate() вызываться не должен
    Deque(const Deque &old_deque){
        // размер нового на 10% больше задейсвованного размера старого
        u_int64_t InitialCapacity = (old_deque.get_number_of_elements()+2)*1.1;

        this->beginning_of_buffer = new Stored[InitialCapacity];
        this->end_of_buffer = beginning_of_buffer + InitialCapacity - 1;
        this->head_of_ring = beginning_of_buffer + InitialCapacity/2;
        this->tail_of_ring = beginning_of_buffer + InitialCapacity/2 +1;
        //осталось только перенести элементы, вызывая конструкторы копирования
        //для внутри лежащих элементов.
        for(Stored& i : old_deque) {
            this->push_back(i);
        }
    };

    //деструктор
    ~Deque(){ //только очищает единственное динамическое поле - массив со значениями
        delete[] beginning_of_buffer; // должны взваться деструкторы того, что лежит в массиве
    };

    //std::vector по значению же хранит всё?
    //тут по логике после извлечения хранимое становится невалидным.
    //без копирования не обойтись.
    void push_front(const Stored &in_value){
        //обычный режим - просто добавили значение в массив,
        // --<====>-----  <[≡]  -<≡===>-----
        *head_of_ring = in_value; //1 копирование
        head_of_ring--;

        // режим перехода на другую сторону
        // <=====>------  <[≡]  ≡=====>-----<
        if (head_of_ring < beginning_of_buffer){
            head_of_ring = end_of_buffer;
        }

        //режим почти переполнения - 1 до начала кольца и 1 после конца кольца совпадают
        // <===========>  <[≡]  ≡===========☓  ≡===========>------------<
        // =====><======  <[≡]  =====☓≡======  =====>------------<≡======
        if (head_of_ring == tail_of_ring){
            elongate();
        }
    };

    Stored pop_front(){
        if (head_of_ring == end_of_buffer){
            if(tail_of_ring == beginning_of_buffer) {
                // затруднённый ошибочный случай
                // >-----------< pop[≡]  error
                throw std::out_of_range("Deque is empty");
            } else {
                // затруднённая работа
                // ≡=====>-----<  pop[≡]  <=====>------
                head_of_ring = beginning_of_buffer;
                return *head_of_ring;
            }
        } else {
            if (head_of_ring + 1 == tail_of_ring){
                // ошибочный случай
                // -----<>------ pop[≡]  error
                throw std::out_of_range("Deque is empty");
            } else {
                // нормальная работа
                // -<≡===>----- pop[≡] --<====>-----
                head_of_ring++;
                return *head_of_ring;
            }
        }
    };

    void push_back(const Stored &in_value){
        // обычный режим - просто добавили значение в массив,
        // --<====>-----  [≡]>  --<====≡>----
        // ==>-----<====  [≡]>  ==≡>----<====
        *tail_of_ring = in_value; //1 копирование
        tail_of_ring++;

        // режим перехода на другую сторону
        // ------<=====>  [≡]>  >-----<=====≡
        if (tail_of_ring > end_of_buffer){
            tail_of_ring = beginning_of_buffer;
        }

        //режим почти переполнения - 1 до начала кольца и 1 после конца кольца совпадают
        // <===========>  [≡]>  ☓===========≡  >------------<===========≡
        // =====><======  [≡]>  =====≡☓======  =====≡>------------<======
        if (head_of_ring == tail_of_ring) {
            elongate();
        }
    };

    Stored pop_back(){
        if (tail_of_ring == beginning_of_buffer){
            if(head_of_ring == end_of_buffer) {
                // затруднённый ошибочный случай
                // >-----------< pop[≡]  error
                throw std::out_of_range("Deque is empty");
            } else {
                // затруднённая работа
                // >-----<=====≡  pop[≡]  ------<=====>
                tail_of_ring = end_of_buffer;
                return *tail_of_ring;
            }
        } else {
            if (tail_of_ring-1 == head_of_ring){
                // ошибочный случай
                // -----<>------ pop[≡]  error
                throw std::out_of_range("Deque is empty");
            } else {
                // нормальная работа
                // --<===≡>---- pop[≡] --<===>-----
                tail_of_ring--;
                return *tail_of_ring;
            }
        }
    };


    //теперь я хочу переопределить итерацию range based for
    //https://mbevin.wordpress.com/2012/11/14/range-based-for/ - используемый материал
    class DequeIterator{ // итератор маскируется под указатель.
        //ведёт себя как Stored *обычный_указатель, но перескакивает с конца массива на начало, зацикливаясь.
    protected:
        Stored *current_value;
        Stored *beginning_of_buffer;
        Stored *end_of_buffer;
    public:
        //конструктор просто запоняет три поля.
        DequeIterator(
            Stored *current_value,
            Stored *beginning_of_buffer,
            Stored *end_of_buffer):
            current_value(current_value),
            beginning_of_buffer(beginning_of_buffer),
            end_of_buffer(end_of_buffer){
        };

        Stored& operator*() {
            return *current_value;
        }
        DequeIterator& operator++() {
            current_value++;
            if(current_value > end_of_buffer){
                current_value = beginning_of_buffer;
            }
            return *this;
        }
        bool operator!=(const DequeIterator& it) const {
            return current_value != it.current_value;
        }
    };

    //методы возвращают итераторы на начало и конец соответственно
    DequeIterator begin() const {
        Stored *first = // возвращаем указатель на первый реально существующий элемент
            (head_of_ring == end_of_buffer?
            //≡===>----<
            beginning_of_buffer:
            //--<≡===>--
            head_of_ring + 1);
        return DequeIterator(
            first,
            beginning_of_buffer,
            end_of_buffer);
    }

    DequeIterator end() const {
        //возвращаем указатель на следующий за последним элемент
        return DequeIterator(
                tail_of_ring,
                beginning_of_buffer,
                end_of_buffer);
    }

    //теперь я хочу переопределить +=, +, =.
    Deque<Stored> &operator+=(Deque<Stored> &other){
        for(const Stored i : other){
            this->push_back(i);
        }
        return *this; // можно сделать: a = (b += c);
    };

    Deque<Stored>& operator=(Deque<Stored> &other) {
        if (this != &other) { // если кто-то напишет "a = a;", лучше всего ничего не делаеть
            // уничтожение массива в том экземпляре, которому присваивают
            delete[] this->beginning_of_buffer;
            // подмена массива на новый
            int InitialCapacity = (other.get_number_of_elements()+2) * 1.1;
            this->beginning_of_buffer = new Stored[InitialCapacity];
            this->end_of_buffer = beginning_of_buffer + InitialCapacity - 1;
            this->head_of_ring = beginning_of_buffer + InitialCapacity/2;
            this->tail_of_ring = beginning_of_buffer + InitialCapacity/2 +1;
            //осталось только перенести элементы, вызывая конструкторы копирования
            //для внутри лежащих элементов.
            for(Stored& i : other) {
                this->push_back(i);
            }
        }
        return *this;
    }

    Deque<Stored> operator+(Deque<Stored> &other){
        //считаем, сколько памяти надо выделить - на 10% больше, чем в двух объектах.
        u_int64_t InitialCapacity = (this->get_number_of_elements() + other.get_number_of_elements() + 2)*1.1;
        //создать новый экземпляр
        Deque<Stored> *sum = new Deque<Stored>;
        for(const Stored i : *this){
            sum->push_back(i); //массив расширяться не будет.
        }
        for(const Stored i : other){
            sum->push_back(i);
        }
        return *sum;
    }

private:

    Stored *beginning_of_buffer = nullptr; // {-}-<====>-----
    Stored *end_of_buffer = nullptr;       // --<====>----{-}
    Stored *head_of_ring = nullptr;        // --{<}====>-----
    Stored *tail_of_ring = nullptr;        // --<===={>}-----

    void elongate(){//увеличение в 2 раза размера буффера.
        // функция вызывается для двух трёх случаев:
        // ☓============  >------------<============
        // ============☓  ============>------------<
        // ======☓======  ======>------------<======
        // не сильно отличается
        // создаём новый массив
        unsigned long new_capasity = (end_of_buffer - beginning_of_buffer + 1) * 2;
        Stored *temporary = new Stored[new_capasity];

        Stored *new_beginning_of_buffer = new Stored[new_capasity]; // {-}-<====>-----
        Stored *new_end_of_buffer = new_beginning_of_buffer + new_capasity - 1; // --<====>----{-}
        Stored *new_head_of_ring = new_end_of_buffer - (end_of_buffer-head_of_ring); // --{<}====>-----
        Stored *new_tail_of_ring = tail_of_ring - beginning_of_buffer + new_beginning_of_buffer; // --<===={>}-----

        //======☓======  --------------------------
        size_t part_size = (tail_of_ring - beginning_of_buffer) * sizeof(Stored);
        std::memcpy(new_beginning_of_buffer, beginning_of_buffer, part_size);
        //≠≠≠≠≠≠<======  ======>-------------------
        part_size = (end_of_buffer - head_of_ring) * sizeof(Stored);
        std::memcpy(new_head_of_ring+1, head_of_ring+1, part_size);
        //≠≠≠≠≠≠-≠≠≠≠≠≠  ======>------------<======
        delete[] beginning_of_buffer;
        //               ======>------------<======
        beginning_of_buffer = new_beginning_of_buffer;
        end_of_buffer = new_end_of_buffer;
        head_of_ring = new_head_of_ring;
        tail_of_ring = new_tail_of_ring;
    };

    //мне нужна служебная функция, которая возвращает количество элементов в очереди
    u_int64_t get_number_of_elements()const {
        u_int64_t capacity = 0;
        if (this->head_of_ring < this->tail_of_ring) {
            // --<====>-----
            capacity = this->tail_of_ring - this->head_of_ring - 1;
        } else {
            // ==>-----<====
            capacity = this->end_of_buffer - this->head_of_ring +
                       this->tail_of_ring - this->beginning_of_buffer;
        }
        return capacity;
    }
};


//вне класса
//Перегрузка операции вывода в поток
template <typename Stored>
std::ostream& operator<<(std::ostream& os, const Deque<Stored>& deque) { //почему не получается сделать const?
    for(const Stored i : deque){
        os << i << ' ';
    }
    return os;
}

#endif //INC_7_DEQUE_H
