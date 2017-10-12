//
// Created by oleg on 10.10.17.
//
#include <stdlib.h>
#include <stdio.h>

#ifndef INC_3_STACK_H
#define INC_3_STACK_H

//стек - первый вошёл/первый вышел
struct stack{
    int size;//размер доступной памяти
    char * array; //указатель на массив значений
    char * after_last_element; //указатель на последнее ещё не задействованное значение
};

// представление
// 0 _ _
// 1 ( )
// 2 [ ]
// 3 { }

//даёшь ООП!
//конструктор stack
struct stack *create_stack();

//деструктор stack
void free_stack(struct stack *self);

int extend_stack_if_needed(struct stack *self);

//главная логика - добавление скобки
//возвращает не 0 в случае неудачи
int process_letter(struct stack *self, char bracket);

#endif //INC_3_STACK_H
