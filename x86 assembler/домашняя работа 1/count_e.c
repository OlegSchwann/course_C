//
// Created by oleg on 24.04.18.
//

int count_e(const char *string, char *arr_e) {
    unsigned int word_i = 0;
    unsigned int char_i = 0;
    char was_delimiter  = 1;
    while (string[char_i] != '\0') {
        if (string[char_i] == ' ') {
            if (was_delimiter == 0) {
                ++word_i; // значит переходим к следующему слову.
                was_delimiter = 1;
            }
        } else {
            was_delimiter = 0;
        }
        if (string[char_i] == 'e') { // если буква
            ++arr_e[word_i];
        }
        ++char_i;
    }
    if (was_delimiter == 0){
       ++word_i;
    } // если заканчивается на пробел, то уже не надо увеличивать количество слов
    return word_i;
};