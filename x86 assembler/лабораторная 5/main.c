// сборка вместе с кодом на assembler.
// gcc ../main.c ../swap_word.s
#include "swap_word.h"
#include <stdio.h>
int main() {
    char inbuffer[256] = {0};
    char outbuffer[256] = {0};
    unsigned int word1 = 0, word2 = 0;
//  спрашиваем у пользователя данные
    printf("Enter a few words, separated by spaces, not more than 254 characters:\n");
    for(int letter = 0, i = 0; i < 255 && (letter = getc(stdin)) != EOF && letter != '\n'; ++i){
        inbuffer[i] = (char)letter;
    }
    printf("Enter first word:\n");
    int err = scanf("%u", &word1);
    if (err != 1){
        fprintf(stderr, "Not unsigned num.\n");
        return 1;
    }
    printf("Enter second word:\n");
    err = scanf("%u", &word2);
    if (err != 1){
        fprintf(stderr, "Not unsigned num.\n");
        return 1;
    }
    swap_word/*and print*/(inbuffer, outbuffer, word1, word2);
    return 0;
}