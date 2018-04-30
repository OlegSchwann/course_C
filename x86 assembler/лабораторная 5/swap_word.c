// получение файла с asm командами.
// gcc -S -masm=intel ../swap_word.c -o ../swap_word.s

// Вариант 16. Дан текст не более 255 символов.
// Слова отделяются друг от друга пробелами.
// Поменять местами пары слов с указанными номерами.

#include <stdio.h>

// принимает указатели на строки источника и назначения.
// они не длиннее 255 char и src точно null-терминированная
// гарантируетсяб что word1 < word2
void swap_word(const char *source, char *destination, unsigned int word1, unsigned int word2) {
    // массив индексов начал слов
    char begin_word_index[128];
    char begins_index;
    char was_separator;
    char current_character;
    char from_indexes[5];
    char to_indexes[5];
    char dest_i;
    char sites_i;
    char src_i;
    // больше переменных не используется.

    // счётчик - индекс ещё не заполненной ячейки в массиве индексов начал слов
    begins_index = 0;
    // флаг "предыдущим символом был разделитель"
    // == true, так как начало строки - тоже разделитель
    was_separator = 1;
    // индекс текущего символа
    current_character = 0;
    begin_parse_loop:
    if (source[current_character] == ' ') {
        was_separator = 1;
        ++current_character;
        goto begin_parse_loop;
    }
    if (source[current_character] == '\0') {
        //последняя ячейка содержит индекс конца строки
        begin_word_index[begins_index] = current_character;
        goto end_parse_loop;
    }
    // если любой другой символ, не ␠, не ␀
    if (was_separator) {
        begin_word_index[begins_index] = current_character;
        ++begins_index;
        was_separator = 0;
        ++current_character;
        goto begin_parse_loop;
    }
    // если символ не после пробела и не в начале строки
    // просто переходим к следующему
    ++current_character;
    goto begin_parse_loop;
    end_parse_loop:
    // на данном этапе в массиве begin_word_index хранятся начала слов в количестве begins_index;
    // теперь копируем в назначение из источника в назначение.
    //            ↓word1           ↓word2
    // =====-====-=========-======-========-=======-=======0
    // [от нулевого до индекса первого меняемого слова)
    //            ↓                ↓
    // ≈≈≈≈≈~≈≈≈≈~=========-======-========-=======-=======0
    // [от индекса второго меняемого слова до индекс второго меняемого слова+1)
    //            ↓                ↓
    // ≈≈≈≈≈~≈≈≈≈~=========-======-≈≈≈≈≈≈≈≈~=======-=======0
    // [от индекса первого меняемого + 1 до индека второго меняемого)
    //            ↓                ↓
    // ≈≈≈≈≈~≈≈≈≈~=========-≈≈≈≈≈≈~≈≈≈≈≈≈≈≈~=======-=======0
    // [от индекcа первого меняемого до индекса индекса первого меняемого + 1)
    //            ↓                ↓
    // ≈≈≈≈≈~≈≈≈≈~≈≈≈≈≈≈≈≈≈~≈≈≈≈≈≈~≈≈≈≈≈≈≈≈~=======-=======0
    // [от индекса второго меняемого до индекса конца begins_index]
    //            ↓                ↓
    // ≈≈≈≈≈~≈≈≈≈~≈≈≈≈≈≈≈≈≈~≈≈≈≈≈≈~≈≈≈≈≈≈≈≈~≈≈≈≈≈≈≈~≈≈≈≈≈≈≈0


    // только тут мы можем провалидировать входные параметры.
    if (word1 > word2) { // word1 должен быть меньше word2
        unsigned int tmp = word1;
        word1 = word2;
        word2 = tmp;
    }
    if (word2 < begins_index) { // если просят поменять существующие слова

        // массив нижних границ копирования, включая копируемый символ.
        from_indexes[0] = 0;
        from_indexes[1] = begin_word_index[word2];
        from_indexes[2] = begin_word_index[word1 + 1];
        from_indexes[3] = begin_word_index[word1];
        from_indexes[4] = begin_word_index[word2 + 1];
        // массив верхних границ копирования, не включая этот символ.
        to_indexes[0] = begin_word_index[word1];
        to_indexes[1] = begin_word_index[word2 + 1];
        to_indexes[2] = begin_word_index[word2];
        to_indexes[3] = begin_word_index[word1 + 1];
        to_indexes[4] = begin_word_index[begins_index];
        // индекс на местоб куда будем копировать в назначении, сквозной.
        dest_i = 0;
        // индекс, какой участок из вышезаданных мы копируем.
        sites_i = 0;
        begin_copy_sites_loop:
        src_i = from_indexes[sites_i];
        begin_copy_char_loop:
        if (src_i < to_indexes[sites_i]) {
            destination[dest_i] = source[src_i];
            ++dest_i;
            ++src_i;
            goto begin_copy_char_loop;
        }
        if (sites_i < 5) {
            ++sites_i;
            goto begin_copy_sites_loop;
        }

    } else {
        // если надо поменять слова за пределами существующих, то просто копируем из источника в назначение.
        src_i = 0;
        dest_i = 0;
        begin_memcpy_loop:
        asm("nop");
        char letter = source[src_i];
        if (letter != 0) {
            destination[dest_i] = letter;
            ++src_i;
            ++dest_i;
            goto begin_memcpy_loop;
        }
    }
    destination[dest_i] = '\0';
}

