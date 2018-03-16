#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/*
Time limit:	14 s
Memory limit:	64 M
Составить программу построчной фильтрации текста.
Суть фильтра — отбор строк, начинающихся с одного из следующих выражений:
«Date:», «From:», «To:», «Subject:».
Текстовые строки подаются на стандартный ввод программы, результат программы должен
подаваться на стандартный вывод.


Программа должна уметь обрабатывать возникающие ошибки (например, ошибки выделения памяти). В случае возникновения ошибки нужно вывести в поток стандартного вывода сообщение "[error]" и завершить выполнение программы.

ВАЖНО! Программа в любом случае должна возвращать 0. Не пишите return -1, exit(1) и т.п. Даже если обнаружилась какая-то ошибка, все равно необходимо вернуть 0! (и напечатать [error] в stdout).
 */


//
// динамический массив для char
// изначально было в отельном файле, но контест поддерживает только 1 файл на проект.

typedef struct { // initialSize = 10
    char *array; // char *array
    int used;    // ↓
    int size;    // 0 1 2 3 4 5 6 7 8 9
} Slice_char;    // + + + + + - - - - -
//                            ↑         ↑
// индексы элементов          used      size

int slice_char_init(Slice_char *self, int initialSize) {
    self->array = (char *) malloc(initialSize * sizeof(char));
    if (self->array == NULL) {
        return 1;
    }
    self->used = 0;
    self->size = initialSize;
    return 0;
}

int slice_char_append(Slice_char *self, char element) {
    if (self->used == self->size) {
        int required_size = self->size * 2;
        char *new_array = (char *) realloc(self->array, required_size * sizeof(char));
        if (new_array == NULL) {
            return 1;
        } // не портим slice, если не можем выделить память
        self->array = new_array;
        self->size = required_size;
    }
    self->array[self->used] = element;
    ++(self->used);
    return 0;
}

void slice_char_free(Slice_char *self) {
    free(self->array);
    self->array = NULL;
    self->used = 0;
    self->size = 0;
}

//
// динамический массив для указателей на char
//

typedef struct {       // initialSize = 10
    Slice_char *array; // char *array
    int used;          // ↓
    int size;          // 0 1 2 3 4 5 6 7 8 9
} Slice_pointer;       // + + + + + - - - - -
//                                  ↑         ↑
// индексы элементов               used      size

int slice_pointer_init(Slice_pointer *self, int initialSize) {
    self->array = (Slice_char *) malloc(initialSize * sizeof(Slice_char));
    if (self->array == NULL) {
        return 1;
    }
    self->used = 0;
    self->size = initialSize;
    return 0;
}

int slice_pointer_append(Slice_pointer *self, Slice_char element) {
    if (self->used == self->size) {
        int required_size = self->size * 2;
        Slice_char *new_array
                = (Slice_char *) realloc(self->array, required_size * sizeof(Slice_char));
        if (new_array == NULL) {
            return 1;
        } // не портим slice, если не можем выделить память
        self->array = new_array;
        self->size = required_size;
    }
    self->array[self->used] = element;
    ++(self->used);
    return 0;
}

void slice_pointer_free(Slice_pointer *self) {
    free(self->array);
    self->array = NULL;
    self->used = 0;
    self->size = 0;
}

bool contains_title(char *string) {
    return (
            strncmp(string, "Date:",    5) == 0 ||
            strncmp(string, "From:",    5) == 0 ||
            strncmp(string, "To:",      3) == 0 ||
            strncmp(string, "Subject:", 8) == 0
    );
}

// функция фильтрации, которой подается на вход
// массив строк, выделенных в динамической памяти и его длина.
// На выходе функция возвращает указатель на NULL-терминированный массив с найденными строками
// (последним элементом массива добавлен NULL для обозначения, что данных больше нет).
Slice_pointer string_filtering(Slice_pointer all_strings) {
    Slice_pointer result;
    slice_pointer_init(&result, 32);
    for (int i = 0; all_strings.array[i].array != NULL; ++i) {
        if (contains_title(all_strings.array[i].array)) {
            slice_pointer_append(&result, all_strings.array[i]);
        }
    }
    Slice_char null_slise;
    null_slise.array = NULL; // необосновано.
    null_slise.used = 0;
    null_slise.size = 0;
    slice_pointer_append(&result, null_slise);
    return result;
}

#define MASSAGE_ERROR printf("[error]")
#define INITIAL_CAPASITY 32

int main() {
    int err = 0; // код возврата для всех append и init, которые могут завершиться с ошибкой.
    int letter = '\0'; // куда считываем символы
    Slice_pointer all_strings; // куда записываем строки
    err = slice_pointer_init(&all_strings, INITIAL_CAPASITY);
    if(err != 0){
        MASSAGE_ERROR;
        return 0;
    }
    Slice_char one_string;  // куда записываем символы текущей строки
    err = slice_char_init(&one_string, INITIAL_CAPASITY);
    if(err != 0){
        MASSAGE_ERROR;
        return 0;
    }
    while (true) {
        letter = getc(stdin);
        if (letter != EOF) {
            if ((char) letter != '\n') {
                err = slice_char_append(&one_string, (char) letter);
                if (err != 0){
                    MASSAGE_ERROR;
                    return 0;
                }
            } else {
                err = slice_char_append(&one_string, '\0');           // завершаем нулём
                if (err != 0){
                    MASSAGE_ERROR;
                    return 0;
                }
                err = slice_pointer_append(&all_strings, one_string); // кладём строку в массив строк
                if(err != 0){
                    MASSAGE_ERROR;
                    return 0;
                }
                // создаём новую строку, куда будем класть следующие символы
                err = slice_char_init(&one_string, INITIAL_CAPASITY);
                if(err != 0){
                    MASSAGE_ERROR;
                    return 0;
                }
            }
        } else {
            err = slice_char_append(&one_string, '\0');
            if (err != 0){
                MASSAGE_ERROR;
                return 0;
            }
            err = slice_pointer_append(&all_strings, one_string);
            if(err != 0){
                MASSAGE_ERROR;
                return 0;
            }
            one_string.array = NULL; // завершаем нулём.
            one_string.used = 0;
            one_string.size = 0;
            err = slice_pointer_append(&all_strings, one_string);
            if (err != 0){
                MASSAGE_ERROR;
                return 0;
            }
            break;
        }
    }
    if (ferror(stdin)) { // EOF возвращается в случае ошибки тоже.
        printf("[error]");
        return 0;
    }
    Slice_pointer strings_with_title = string_filtering(all_strings);
    for (int i = 0; strings_with_title.array[i].array != NULL; ++i) {
        printf("%s\n", strings_with_title.array[i].array);
    }
    // очистка
    // массив указателей на нужные строки с заголовками
    slice_pointer_free(&strings_with_title);
    deleting_one_string_and_all_strings_with_contents:
    slice_char_free(&one_string);
    // очистка: каждую строку отдельно
    for (int i = 0; all_strings.array[i].array != NULL; ++i) { // При ошибке массив может оказаться не нультерминированной
        slice_char_free(&(all_strings.array[i]));
    }
    // массив указателей на все строки
    slice_pointer_free(&all_strings);
    return 0;
}
