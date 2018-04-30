// Вариант 16.
// Дан текст 8 слов, разделенных пробелом.
// Определить количество повторений буквы Е в каждом слове.

// http://indigobits.com/assembler/22-vypolnenie-komand-s-prefiksami-napravlenie-obrabotki-cepochek.html
// https://en.wikipedia.org/wiki/X86_assembly_language
// https://www.sourceware.org/binutils/docs-2.12/as.info/Pseudo-Ops.html#Pseudo%20Ops
// https://ru.wikibooks.org/wiki/Ассемблер_в_Linux_для_программистов_C
// ftp://ftp.gnu.org/old-gnu/Manuals/gas/html_chapter/as_7.html

#include <stdio.h>

#include <unistd.h>
#include "count_e.h"
#include "string.h"

char *help_messege =
        "Программа считывает строку, и определяет количество повторений буквы Е в каждом слове.\n"
        "Любопытно только то, что основная часть написана на assembler,\n"
        "с использованием сверхоптимизированных инструкций работы со строками,\n"
        "которые доступны в архитектуре x86. Семёнов (Shwann) Максим Иу6-43.\n";

// преобразование числа в строку, base 10.
size_t itoa(unsigned int value, char *str) {
    char *wstr = str;
    // Conversion. Number is reversed.
    do {
        *wstr++ = (char) (value % 10) + '0';
    } while (value /= 10);
    *wstr = '\0';
    size_t len = wstr - str;
    // Reverse string
    --wstr;
    char aux;
    while (wstr > str) {
        aux = *wstr;
        *wstr-- = *str;
        *str++ = aux;
    }
    return len;
}

int main(int argc, char *argv[], char *envp[] )  {
    if (argc > 1){                    // любой аргумент выводит справку.
        write(0, help_messege, 508);
        return 0;
    }
    char read_buffer[1024] = {0};           // буфер для чтения входной строки
    char number_e[513] = {0};               // буфер для количества 'e'
    char out_num_buffer[10] = {0};          // буфер для преобразования числа в строку.
    ssize_t really_read = read(0, read_buffer, 1023); // считываем в буфер из stdin с использованием системного вызова
    if(really_read == -1){
        write(1, "Read_error\n", 11);
        return 1;
    } else {
        read_buffer[really_read-1] = '\0';            // нультерминируем полученную строку
    }
    int word_i = count_e(read_buffer, number_e);
    for(int i = 0; i < word_i; ++i){
        size_t num_size = itoa(number_e[i], out_num_buffer);
        write(1, out_num_buffer, num_size);
        write(1, " ", 1);
    }
    write(1, "\b\n", 2); // убераем последний пробел и переводим строку.
    return 0;
}