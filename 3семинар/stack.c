//
// Created by oleg on 10.10.17.
//

#include <wchar.h>
#include <stddef.h>
#include "stack.h"

// представление
// 0 _ _
// 1 ( )
// 2 [ ]
// 3 { }

//даёшь ООП!
//конструктор stack
inline const int stack_start_size = 2;

struct stack *create_stack(){
    struct stack *self = malloc(sizeof(struct stack));
    if(self == NULL){
        return NULL;
    }
    self->array = calloc(stack_start_size, sizeof(self->array[0]));
    if(self->array == NULL){
        free(self);
        return NULL;
    }
    self->after_last_element = self->array;
    self->size = stack_start_size;
    return self;
}

//деструктор stack
void free_stack(struct stack *self){
    free(self->array);
    free(self);
    self = NULL;
}
int* a ;

int extend_stack_if_needed(struct stack *self){
    //увеличивает массив в 2 раза или грамотно падает, если не может так жить
    if(self->array + self->size <= self->after_last_element){
        ptrdiff_t position = self->after_last_element - self->array;
        char *new_array = realloc(self->array, self->size * 2);
        if(new_array == NULL){
            perror("нет памяти");
            return 1;
        }
        self->array = new_array;
        self->after_last_element = self->array + position;
        self->size *= 2;
    }
    return 0;
}

//главная логика - добавление скобки
//возвращает не 0 в случае неудачи
int process_letter(struct stack *self, char bracket){
    int err = 0; // код ошибки
    switch(bracket){
        case '(':
            *(self->after_last_element) = 1;
            self->after_last_element++;
            if((err = extend_stack_if_needed(self)) != 0){
                return 5;
            }
            break;
        case '[':
            *(self->after_last_element) = 2;
            self->after_last_element++;
            //проверка after_last_element < array+size и увеличение
            if((err = extend_stack_if_needed(self)) != 0){
                return 5;
            }
            break;
        case '{':
            *(self->after_last_element) = 3;
            self->after_last_element++;
            //проверка after_last_element < array+size и увеличение
            if((err = extend_stack_if_needed(self)) != 0){
                return 5;
            }
            break;
        case ')':
            //всё хорошо, если предыдущий элемент перед after_last_element существует
            //и код равен 1
            if((self->after_last_element != self->array)
               && *(self->after_last_element - 1) == 1){
                *(self->after_last_element--) = 0;
            } else {
                return 1;
            }
            break;
        case ']':
            //всё хорошо, если предыдущий элемент перед after_last_element существует
            //и код равен 2
            if((self->after_last_element != self->array)
               && *(self->after_last_element - 1) == 2){
                *(self->after_last_element--) = 0;
            } else {
                return 2;
            }
            break;
        case '}':
            //всё хорошо, если предыдущий элемент перед after_last_element существует
            //и код равен 3
            if((self->after_last_element != self->array)
               && *(self->after_last_element - 1) == 3){
                *(self->after_last_element--) = 0;
            } else {
                return 3;
            }
            break;
        default: //мусор дали
            return 4;
    }
    return 0;
}