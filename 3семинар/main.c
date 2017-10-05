/*Написать программу, которая в качестве аргумента командной строки принимает имя текстового файла, содержащего строку символов, в которой записано выражение со скобками трёх типов: [ ], { }, ( ). Необходимо определить, правильно ли расставлены скобки (не учитывая остальные символы) и напечатать на стандартный выходной поток результат. При реализации алгоритма необходимо использовать стек. Например, в выражении [()]{} скобки расставлены правильно, а в выражениях ][ и [({)]} неправильно.*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

inline const int stack_start_size = 2;

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

int extend_stack_if_needed(struct stack *self){
    //увеличивает массив в 2 раза или грамотно падает, если не может так жить
    if(self->array + self->size <= self->after_last_element){
        int position = self->after_last_element - self->array;
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
int add_elem(struct stack *self, char bracket){
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

int main(int argc, char * argv[]) {
    if (argc != 2){ //толко 1 аргумент - путь до файла.
        perror("неверное количество аргументов/n");
        return 1;
    }
    FILE *input_file = fopen(argv[1], "r");
    if(input_file == NULL){
        if(errno == ENOENT){
            perror("нет такого файла\n");
        } else if(errno == EACCES) {
            perror("нет доступа\n");
        }
        return errno;
    }
    //создадим стек
    struct stack * stack_ptr = create_stack();
    if(stack_ptr == NULL){
        perror("нет памяти");
        return 1;
    }
    int letter = '\0';
    int i = 0;
    while(( letter = getc(input_file)) != EOF){
        i++;
        if(letter == '\n'){
            continue;
        }
        int err = add_elem(stack_ptr, letter);
        if(err != 0){
            if(err == 5){
                perror("нет памяти");
                free_stack(stack_ptr);
                return 1;
            }
            printf("файл %s не является корректной скобочной последовательностью, ошибка %d на позиции %d, на символе %c\n", argv[1], err, i, letter);
            return 0;
        }
    }
    if(ferror(input_file)){
        perror("ошибка во время чтения файла");
        return 1;
    }
    printf("файл %s является корректной скобочной последовательностью.\n", argv[1]);
    fclose(input_file);
    free_stack(stack_ptr);
    return 0;
}