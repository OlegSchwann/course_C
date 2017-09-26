/* Домашнее задание No1
Написать программу, которая в качестве аргументов командной строки принимает заданное слово и
имя текстового файла (документа). Документ состоит из предложений, разделителями которых являются
точка, восклицательный знак и вопросительный знак.
Необходимо для этого слова посчитать медиану его вхождений в предложения документа – т.е. надо
найти такое количество вхождений x заданного слова в предложения, что количество предложений, в
которые слово входило не более x раз, максимально близко к количеству предложений, в которые это
слово входило больше x раз.
Слова во входном файле разделяются символами, для которых библиотечные функции isspace() и
ispunct() возвращают ненулевое значение.*/

#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <stdlib.h>
/*#include <ctype.h>*/


char isspace(char letter){
    return (letter == ' ') || (letter == ',') || (letter == '\n');
}

char ispunct(char letter){
    return (letter == '.') || (letter == '!') || (letter == '?');
}

int compare (const void * elem1, const void * elem2)
{
    char f = *((char *)elem1);
    char s = *((char *)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

float megiana(char *entrance, int last_position){
    /* Sort NMEMB elements of BASE, of SIZE bytes each,
    using COMPAR to perform the comparisons.  */
    qsort(entrance, last_position* sizeof(entrance[0]), sizeof(entrance[0]), compare);

    if(last_position % 2 == 0){ // чётное количество элементов
        int midlle = (last_position - 1) / 2;
        float mediana = (entrance[midlle] + entrance[midlle+1]) / 2.0;
        return mediana;
    } else {
        float mediana = entrance[last_position / 2];
        return mediana;
    }
}

int main(int argc, char *argv[]) { //аргументы командной строки - ключевое слово, путь до файла
    assert(argc == 3); // проверяем наличие аргументов

    //переносим из аргумента командной строки ключевое слово в новый массив
    int key_len = strlen(argv[1])+1;
    char key_word[key_len];
    for(int i = 0; i < key_len; i++){
        key_word[i] = argv[1][i];
        assert(!isspace(key_word[i])); // проверяем, что в ключевом слове только буквы
        assert(!ispunct(key_word[i]));
    }
    key_word[key_len-1] = '\0';

    int number_of_sentence = 1;
    char *entrance = realloc(NULL, number_of_sentence * sizeof(char)); //Ячейки - сколько раз ключевое слово входит в предложение с этим номером.
    for(int i = 0; i < number_of_sentence; i++){
        entrance[i] = 0;
    }
    int pos_in_key = 0, pos_in_entrance = 0; // буква, номер предложения на которых мы остановились.

    FILE *input_file = fopen(argv[2], "r"); //открываем файл
    assert(input_file);//проверка, не равен ли указатель 0.

    /*смысл:
    берём букву
    это пробел -
        если совпадает с ключём-
            прибавляем к вхождению в предложение 1
            позиция ключа снова в 0
    это знак пунктуации
        если совпадает с ключём-
            прибавляем к вхождению в предложение 1
            позиция ключа снова в 0
            номер предложения ++
    это буква
        совпадает с буквой ключа на нашей позиции
            положение в ключе++
        не совпадает с буквой ключа
            положение в 0
    */

    char letter = '\0';
    while ((letter = getc(input_file)) != EOF){
        if(ispunct(letter)){
            if (pos_in_key == sizeof(key_word) - 1) {
                putc('+', stdout);
                entrance[pos_in_entrance]++;
                pos_in_key = 0;
            }
            printf("%d", entrance[pos_in_entrance]);
            pos_in_entrance++;
            if(pos_in_entrance >= number_of_sentence){
                number_of_sentence *= 2;
                entrance = realloc(entrance, number_of_sentence * sizeof(entrance[0]));
            };
        } else if(isspace(letter)){
            if (pos_in_key == sizeof(key_word) - 1) {
                putc('+', stdout);
                (entrance[pos_in_entrance]) = 1;
                pos_in_key = 0;
            }
        } else if (key_word[pos_in_key] == letter) {
            pos_in_key++;
        } else { //если не совпадает, то пусть буква будет \0, что бы <префикс_ключ> не распознавались как <ключ>
            pos_in_key = 0;
        }
        putc(letter, stdout);
    }

    /*for(int i = 0; i < pos_in_entrance; i++){
        printf("%d ", entrance[i]);
    }*/

    printf("\nмедиана вхождения слова %s = %.2f", key_word, megiana(entrance, pos_in_entrance));

    return 0;
}