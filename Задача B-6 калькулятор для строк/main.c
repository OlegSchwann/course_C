/*
Задача B-6. Программа калькулятор для строк
Time limit:	14 s
Memory limit:	64 M

Разработайте программу-калькулятор, умеющую вычислять арифметические выражения над строками.
На стандартный поток ввода программы подается входное выражение, а результат вычислений программа должна вывести на стандартный поток вывода.

Строка - это заключенный в двойные кавычки произвольный набор символов.
Например,
"1", "123", "zz!@#111' ad x" - строки,
'asd', "asddas - не строки.
"" - пустая строка, допустимо.


Поддерживаемые операции: '+' - конкатенация, '*' - умножение, '()' - задание приоритета вычислений.
Конкатенировать (складывать) можно только строки! Иначе необходимо вывести "[error]".
Примеры:
"123" + "456", ответ - "123456".
"123" + a: ошибка, необходимо вывести "[error]".

Умножать можно только строку на число! Иначе необходимо вывести "[error]".
Примеры:
"123" * 1, ответ - "123".
"123" * 2, ответ - "123123".
"123" * 3 ответ - "123123123" и т.д.
"123" * a: ошибка, необходимо вывести "[error]".

Между операциями и строками может быть произвольное количество пробелов - их необходимо пропускать.
Например,
"11"     * 2 +     ( "22" + "33" ) - валидное выражение, результат тут будет "11112233".
При вычислениях должны учитываться приоритеты операций (в том числе, заданные вручную при помощи круглых скобочек).

В случае, если введенное выражение содержит ошибку (невалидное выражение), необходимо вывести в поток стандартного вывода сообщение "[error]" и завершить выполнение программы.

ВАЖНО! Программа в любом случае должна возвращать 0. Не пишите return -1, exit(1) и т.п. Даже если обнаружилась какая-то ошибка, все равно необходимо вернуть 0! (и напечатать [error] в stdout).
Examples
Input	                              Output
"1" * 2 + 2*(  "2" +  " "*2 +  "3")   "112  32  3"
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define INITIAL_SIZE 32

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

int slice_char_init(Slice_char *self) {
    self->array = (char *) malloc(INITIAL_SIZE * sizeof(char));
    if (self->array == NULL) {
        return 1;
    }
    self->used = 0;
    self->size = INITIAL_SIZE;
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


// типы данных для Node.type
#define T_error           0 // вернём его, если будет ошибка выделения памяти. Не надо ничего нультерминировать, кроме строк
#define T_string         1
#define T_num            2
#define T_concatenation  3
#define T_multiplication 4
#define T_open_bracket   5
#define T_close_bracket  6

struct Node {
    int type; // типы значений, как указано выше
    void *element; // ссылка на строку или на int
};

//
// динамический массив для char
// изначально было в отельном файле, но контест поддерживает только 1 файл на проект.

typedef struct { // initialSize = 10
    struct Node *array; // char *array
    int used;    // ↓
    int size;    // 0 1 2 3 4 5 6 7 8 9
} Slice_node;    // + + + + + - - - - -
//                            ↑         ↑
// индексы элементов          used      size

int slice_node_init(Slice_node *self) {
    self->array = (struct Node *) malloc(INITIAL_SIZE * sizeof(struct Node));
    if (self->array == NULL) {
        return 1;
    }
    self->used = 0;
    self->size = INITIAL_SIZE;
    return 0;
}

int slice_node_append(Slice_node *self, struct Node element) {
    if (self->used == self->size) {
        int required_size = self->size * 2;
        struct Node *new_array =
                (struct Node *) realloc(self->array, required_size * sizeof(struct Node));
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

// чтоб можно было использовать в качестве стека, нужно иметь метод, возвращающий текущую верхушку стека
// выталкивающий последнее значение
struct Node slice_node_top(Slice_node *self) {
    if (self->used != 0) {
        return self->array[self->used - 1];
    } else {
        // Как передавать ошибку в случае отсутствия?
        // Возвращать node нулевого типа.
        struct Node error_node = {
                T_error, NULL
        };
        return error_node;
    }
}

// метод, выкидывающий верхушку стека
// если стек пуст, возвратим T_error
struct Node slice_node_pop(Slice_node *self) {
    if (self->used != 0) {
        --(self->used);
        return self->array[self->used];
    } else {
        // Как передавать ошибку в случае отсутствия?
        // Возвращать node нулевого типа.
        struct Node error_node = {
                T_error, NULL
        };
        return error_node;
    }
}

void slice_node_free(Slice_node *self) {
    free(self->array);
    self->array = NULL;
    self->used = 0;
    self->size = 0;
}

/* просто отладочная печать содержимого стека.
void print_stack(Slice_node slise_node) {
    for (int i = 0; i < slise_node.used; ++i) {
        switch (slise_node.array[i].type) {
            case T_error:
                printf("[error]");
                break;
            case T_string:
                printf("\"%s\" ", (char *) slise_node.array[i].element);
                break;
            case T_num:
                printf("%d ", *(unsigned int *) slise_node.array[i].element);
                break;
            case T_concatenation:
                printf("+ ");
                break;
            case T_multiplication:
                printf("* ");
                break;
            case T_open_bracket:
                printf("( ");
                break;
            case T_close_bracket:
                printf(") ");
                break;
            default:
                printf("[error]");
        }
    }
}*/

// токенизатор, разбивает строку на токены,
// кладём созданный токем в slise
// типы данных
// T_error
// T_string         "a"
// T_num            1234567890
// T_concatenation  +
// T_multiplication *
// T_open_bracket   (
// T_close_bracket  )
// Возвращаем ссылку на slise токенов
// или Null в случае ошибки
Slice_node *tokenization(Slice_char input_string) {
    struct Node current_token;
    // динамический массив всех токенов
    Slice_node *all_tokens = malloc(sizeof(Slice_node));
    slice_node_init(all_tokens); // Не меньше sizeof(unsigned int)
    Slice_char current_string; // текущая строка, в которую добавляем литерал строки
    slice_char_init(&current_string);
    for (int i = 0; input_string.array[i] != '\0'; ++i) {
        // вытаскиваем строку
        if (input_string.array[i] == '\"') {
            while (true) {
                ++i;
                if (input_string.array[i] != '\0') {
                    if (input_string.array[i] != '\"') {
                        slice_char_append(&current_string, input_string.array[i]);
                    } else {
                        slice_char_append(&current_string, '\0'); // текущую строку завершаем
                        // делаем токен типа строки
                        current_token.type = T_string;
                        current_token.element = current_string.array;
                        // добавляем его в массив токенов
                        slice_node_append(all_tokens, current_token);
                        // и создаём новую строку, в которую будем класть следующие символы или числа.
                        slice_char_init(&current_string);
/*      ┌────────────*/ break;
/*      ▼    */     }
                } else { // ошибочная ситуация - конец строки встретился раньше, чем кавычка оказалась закрыта
                    // очищаем текущую строку, очищаем все строки у токенов
                    // выводим ошибку
                    // заполняем результат T_error токеном
                    // выходим из функции
                    goto clean_after_error;
                }
            }
        } else
            //вытаскиваем цифры
        if ('0' <= input_string.array[i] && input_string.array[i] <= '9') {
            // цифры превращаем в unsigned int
            // записываем в эту же строку бинарное представление, что бы не перевыделять
            unsigned int current_num = 0;
            while ('0' <= input_string.array[i] && input_string.array[i] <= '9') {
                current_num = current_num * 10 + (input_string.array[i] - '0');
                ++i;
            }
            // i указывает на следующий после числа элемент
            --i;
            *(unsigned int *) current_string.array = current_num;
            // делаем токен типа строки
            current_token.type = T_num;
            current_token.element = current_string.array;
            // добавляем его в массив токенов
            slice_node_append(all_tokens, current_token);
            // делаем новую строку
            slice_char_init(&current_string);
        } else
            // вытаскиваем знак +
        if (input_string.array[i] == '+') {
            current_token.type = T_concatenation;
            current_token.element = NULL;
            slice_node_append(all_tokens, current_token);
        } else
            // вытаскиваем знак *
        if (input_string.array[i] == '*') {
            current_token.type = T_multiplication;
            current_token.element = NULL;
            slice_node_append(all_tokens, current_token);
        } else
            // вытаскиваем открывающую скобку
        if (input_string.array[i] == '(') {
            current_token.type = T_open_bracket;
            current_token.element = NULL;
            slice_node_append(all_tokens, current_token);
        } else
            // вытаскиваем закрывающую скобку
        if (input_string.array[i] == ')') {
            current_token.type = T_close_bracket;
            current_token.element = NULL;
            slice_node_append(all_tokens, current_token);
        } else
            // пропускаем пробельные символы
        if (input_string.array[i] == ' ' || input_string.array[i] == '\n') {
            // ничего
        } else {
            // не строка, не число, не оператор, не пробел - ошибка!
            clean_after_error:
            slice_char_free(&current_string);
            for (int j = 0; j < all_tokens->used; ++j) {
                free(all_tokens->array[j].element);
            }
            slice_node_free(all_tokens);
            free(all_tokens);
            return NULL;
        }
    }
    // удаляем последнюю выделенную строку, не привязанную к массиву всех токенов
    slice_char_free(&current_string);
    return all_tokens;
}

// алгоритм сортировочной станции
/*
Пока не все токены обработаны:
    Прочитать токен.
        Если токен — число, то добавить его в очередь вывода.
        Если токен — оператор, то:
            если приоритет нового
                больше, чем на стеке      --> кладём новый на стек
                равен тому, что на стеке  --> кладём новый на стек
                меньше того, что на стеке --> вытаскиваем из стека оператор,
                    пока не наткнёмся на такой оператор, что приоритет нового больше или равен приоритету верхушки стека
                    Добавляем новый в стек
                не важно какой, на стеке ничего нет --> кладём новый на стек
        Если токен — открывающая скобка, то положить его в стек.
        Если токен — закрывающая скобка:
            Пока токен на вершине стека не является открывающей скобкой, перекладывать операторы из стека в выходную очередь.
            Выкинуть открывающую скобку из стека, но не добавлять в очередь вывода.
            Если стек закончился до того, как был встречен токен открывающая скобка, то в выражении пропущена скобка.
Если больше не осталось токенов на входе:
    Пока есть токены операторы в стеке:
        Если токен оператор на вершине стека — скобка, то в выражении присутствует незакрытая скобка.
    Переложить оператор из стека в выходную очередь.
Конец.*/

// принимаем токены в инфиксной нотации, возвращаем без скобок в постфиксной нотации
// или возвращаем NILL, если была допущена ошибка во входных данных
Slice_node *build_postfix_notation(const Slice_node *const infix_notation) {
    Slice_node *result = malloc(sizeof(Slice_node)); // выходной массив
    slice_node_init(result); // инициируем выходной массив.
    Slice_node stack_of_operators; // стек для хранения операторов и скобок
    slice_node_init(&stack_of_operators);
    for (int i = 0; i < infix_notation->used; ++i) {
        struct Node current_token = infix_notation->array[i];
        switch (current_token.type) {
            case T_string:
                // Если токен — строка, то добавить его в очередь вывода.
                slice_node_append(result, current_token);
                break;
            case T_num:
                // Если токен — число, то добавить его в очередь вывода.
                slice_node_append(result, current_token);
                break;
            case T_concatenation:
                // Если токен — '+', то:
                // если на стеке ничего нет или
                //      на стеке открывающая скобка
                //      на стеке '+' --> кладём '+' на стек
                // если на стеке '*' --> вытаскиваем из стека оператор '*' и кладём в результат,
                //     повторяем, пока не наткнёмся на конец или на +
                //     Добавляем новый в стек
                if (slice_node_top(&stack_of_operators).type == T_concatenation ||
                    slice_node_top(&stack_of_operators).type == T_open_bracket ||
                    slice_node_top(&stack_of_operators).type == T_error) {
                    slice_node_append(&stack_of_operators, current_token);
                } else {
                    while (slice_node_top(&stack_of_operators).type == T_multiplication) { // пока умножение сверху
                        // перекладываем все знаки умножения в выходной массив, пока не наткнёмся на скобку, или конец, или '+'
                        slice_node_append(result, slice_node_pop(&stack_of_operators));
                    }
                    slice_node_append(&stack_of_operators, current_token);
                }
                break;
            case T_multiplication:
                // Если токен — '*':
                // так как приоритет больше любого другого --> кладём новый на стек
                slice_node_append(&stack_of_operators, current_token);
                break;
            case T_open_bracket:
                // Если токен — открывающая скобка, то положить его в стек для операторов.
                slice_node_append(&stack_of_operators, current_token);
                break;
            case T_close_bracket:
                // Если токен — закрывающая скобка:
                // Пока токен на вершине стека не является открывающей скобкой,
                // перекладывать операторы из стека в выходную очередь.
                // Выкинуть открывающую скобку из стека, но не добавлять в очередь вывода.
                // Если стек закончился до того, как был встречен токен открывающая скобка,
                // то в выражении пропущена скобка.
                for (struct Node last_operator = slice_node_pop(&stack_of_operators);
                     last_operator.type != T_open_bracket;
                     last_operator = slice_node_pop(&stack_of_operators)) {
                    if (last_operator.type != T_error) {
                        slice_node_append(result,
                                          last_operator); // без копирования самой строки, копируются только 2 int
                    } else { // закрывающая скобка без открывающий
                        // очищаем массив результата, не трогая строки с данными токенов
                        slice_node_free(result);
                        free(result);
                        return NULL;
                    }
                }
                break;
            default:
                // пришёл ошибочный токен
                slice_node_free(result);
                free(result);
                return NULL;
        }
    } // больше не осталось токенов на входе:
    // Пока есть токены операторы в стеке:
    // Если токен оператор на вершине стека — скобка, то в выражении присутствует незакрытая скобка.
    //     Переложить оператор из стека в выходную очередь.
    for (struct Node last_operator = slice_node_pop(&stack_of_operators);
         last_operator.type != T_error;
         last_operator = slice_node_pop(&stack_of_operators)) {
        if (last_operator.type != T_open_bracket) {
            slice_node_append(result, last_operator);
        } else {
            slice_node_free(result);
            free(result);
            return NULL;
        }
    }
    // очищается стек операторов, а стек с результатом возвращается
    slice_node_free(&stack_of_operators);
    return result;
}

// операция сложения строк
// выделяет новую память и возвращает новую строку, собранную из двух, не портит исходные.
// решил избежать ошибок и сделать строки неизменяемыми, как в python.
char *concat(const char *const string_1, const char *const string_2) {
    size_t len1 = strlen(string_1);
    size_t len2 = strlen(string_2);
    char *result = (char *) malloc(len1 + len2 + 1);
    if (result == NULL) {
        return NULL;
    }
    memcpy(result, string_1, len1);
    memcpy(result + len1, string_2, len2 + 1);
    return result;
}

// умножает строку на число: "ab" * 3 == "ababab"
// выделяет новую память.
char *multiply(const char *const string, const unsigned int num) {
    size_t len = strlen(string);
    char *result = (char *) malloc(len * num + 1);
    if (result == NULL) {
        return NULL;
    }
    for (unsigned int i = 0; i < num; ++i) {
        memcpy(result + len * i, string, len + 1);
    }
    return result;
}

// стековая машина для вычисления обратной польской записи
// возвращает или указатель на Node type T_string
// или NULL в случае ошибки
// Суть алгоритма
// для каждого входного символа польской нотации
//     Если на вход подан операнд,
//         он помещается на вершину стека.
//     Если на вход подан знак операции,
//         то соответствующая операция выполняется над требуемым количеством значений,
//         извлечённых из стека, взятых в порядке добавления.
//         Результат выполненной операции кладётся на вершину стека.
// После полной обработки входного набора символов результат вычисления выражения лежит на вершине стека.
struct Node *compute_on_stack(const Slice_node *const postfix_notation) {
    Slice_node stack_for_calculations;
    slice_node_init(&stack_for_calculations);
    for (int i = 0; i < postfix_notation->used; i++) {
        struct Node current_token = postfix_notation->array[i];
        switch (current_token.type) {
            case T_string:
                // что бы не портить входные данные и избежать двух указателей на одну облать памяти
                // (и двойного освобождения в случае аварийной ситуации)
                // копируем каждый раз данные, содержащиеся по указателю Node.element
                // в новую область.
            {
                struct Node copied_node;
                copied_node.type = T_string;
                size_t current_token_string_len = strlen(current_token.element) + 1;
                copied_node.element = malloc(current_token_string_len);
                // переносим число, хранящееся в токене в новый токен
                memcpy(copied_node.element, current_token.element, current_token_string_len);
                slice_node_append(&stack_for_calculations, copied_node);
            }
                break;
            case T_num:
                // что бы не портить входные данные и избежать двух указателей на одну облать памяти
                // (и двойного освобождения в случае аварийной ситуации)
                // копируем каждый раз данные, содержащиеся по указателю Node.element
                // в новую область.
            {
                struct Node copied_node;
                copied_node.type = T_num;
                copied_node.element = malloc(sizeof(unsigned int));
                // переносим число, хранящееся в токене в новый токен
                *(unsigned int *) copied_node.element = *(unsigned int *) current_token.element;
                slice_node_append(&stack_for_calculations, copied_node);
            }
                break;
            case T_concatenation: {
                // применяем сложение к двум операндам, лежащим на вершине стека.
                // выделяем память под новую строку и не забываем удалить выделенное под операнды
                // ниже лежащий в стеке должен быть первым слагаемым.
                struct Node second_node = slice_node_pop(&stack_for_calculations);
                if (second_node.type != T_string) {
                    free(second_node.element);
                    goto clean_after_error;
                }
                struct Node first_node = slice_node_pop(&stack_for_calculations);
                if (first_node.type != T_string) { // фатальная ошибка типа
                    free(second_node.element);
                    free(first_node.element);
                    goto clean_after_error;
                }
                struct Node node_of_concated_string;
                node_of_concated_string.type = T_string;
                node_of_concated_string.element = concat(first_node.element, second_node.element);
                free(first_node.element);
                free(second_node.element);
                slice_node_append(&stack_for_calculations, node_of_concated_string);
            }
                break;
            case T_multiplication: {
                struct Node first_node = slice_node_pop(&stack_for_calculations);
                struct Node second_node = slice_node_pop(&stack_for_calculations);
                struct Node node_of_multiplied_string;
                node_of_multiplied_string.type = T_string;
                if (first_node.type == T_string && second_node.type == T_num) {
                    node_of_multiplied_string.element =
                            multiply(first_node.element, *(unsigned int *) second_node.element);
                } else if (first_node.type == T_num && second_node.type == T_string) {
                    node_of_multiplied_string.element =
                            multiply(second_node.element, *(unsigned int *) first_node.element);
                } else { // прислали некорректные аргументы
                    free(first_node.element);
                    free(second_node.element);
                    goto clean_after_error;
                }
                free(first_node.element);
                free(second_node.element);
                slice_node_append(&stack_for_calculations, node_of_multiplied_string);
            }
                break;
            default: // только операции и операнды допустимы в обратной польской записи
                // очищаем всё выделенное в функции и возвращаем NULL
            clean_after_error:
                for (int j = 0; j < stack_for_calculations.used; ++j) {
                    free(stack_for_calculations.array[j].element);
                    // если там лежит операнд, Node.element == NULL, по стандарту free(NULL); - корректный код.
                }
                slice_node_free(&stack_for_calculations);
                return NULL;
        }
    }
    // после вычисления результат лежит на верхушке стека
    struct Node *result = malloc(sizeof(struct Node));
    *result = slice_node_pop(&stack_for_calculations);
    slice_node_free(&stack_for_calculations);
    return result;
}

int main() {
    // считываем в динамический массив введённые данные
    Slice_char input_string;
    slice_char_init(&input_string);
    while (true) {
        int letter = getc(stdin);
        if (letter != EOF /*'\n'*/) {
            slice_char_append(&input_string, (char) letter);
        } else {
            slice_char_append(&input_string, '\0');
            break;
        }
    }
    if (ferror(stdin)) { // EOF возвращается в случае ошибки тоже.
        printf("[error]");
        slice_char_free(&input_string);
        return 0;
    }

    Slice_node *all_tokens = tokenization(input_string);
    if (all_tokens == NULL) {
        slice_char_free(&input_string);
        printf("[error]");
        return 0;
    }
    slice_char_free(&input_string);

    // алгоритм сортировачной станции
    Slice_node *postfix_notation = build_postfix_notation(all_tokens);
    if (postfix_notation == NULL) {
        for (int i = 0; i < all_tokens->used; ++i) {
            free(all_tokens->array[i].element);
        }
        slice_node_free(all_tokens);
        printf("[error]");
        return 0;
    }

    // а те строки, на которые ссылался all_tokens.array[].element --> разделяемые с
    //                            postfix_notation.array[].element, не надо их трогать
    slice_node_free(all_tokens);
    free(all_tokens);

    // отладочный вывод
    // printf("обратная польская запись: ");
    // print_stack(*postfix_notation);
    // printf("\n");

    // стековую машину для вычисления обратной польской записи.
    struct Node *result_node = compute_on_stack(postfix_notation);
    if (result_node == NULL) {
        for (int i = 0; i < postfix_notation->used; ++i) {
            free(postfix_notation->array[i].element);
        }
        slice_node_free(postfix_notation);
        free(postfix_notation);
        printf("[error]");
        return 0;
    }

    // очистка всей выделенной памяти.
    // postfix_notation --> Slice_node --> Slice_node.array --> Node.array[].element
    for (int i = 0; i < postfix_notation->used; ++i) {
        free(postfix_notation->array[i].element);
    }
    slice_node_free(postfix_notation);
    free(postfix_notation);

    printf("\"%s\"", (char *) result_node->element);
    free(result_node->element);
    free(result_node);
    return 0;
}