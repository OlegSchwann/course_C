/*Написать программу, которая в качестве аргумента командной строки принимает имя текстового файла, содержащего строку символов, в которой записано выражение со скобками трёх типов: [ ], { }, ( ). Необходимо определить, правильно ли расставлены скобки (не учитывая остальные символы) и напечатать на стандартный выходной поток результат. При реализации алгоритма необходимо использовать стек. Например, в выражении [()]{} скобки расставлены правильно, а в выражениях ][ и [({)]} неправильно.*/

#include <stdio.h>
//#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "stack.h"



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
        if(isspace(letter)){
            continue;
        }
        int err = process_letter(stack_ptr, letter);
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