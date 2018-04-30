# сигнатура вызова
# void swap_word(char *src, char *dest, unsigned int word1, unsigned int word2);
	.intel_syntax noprefix
	.section	.rodata # только для чтения
printf_format_string:
	.string	"%s"
	# глобальные переменные. Так проще, чем размещать их на стеке сложно отслеживать адреса.
	.section .data
begin_word_index:  # char begin_word_index[128];
    .space 128
from_indexes:      # char from_indexes[5];
    .space 5
to_indexes:        # char to_indexes[5];
    .space 5
# больше переменных не используется
	.text
	.globl	swap_word
	.type	swap_word, @function
swap_word:
.LFB0:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	QWORD PTR [rbp- 8], rdi  # что это?
	mov	QWORD PTR [rbp-16], rsi
	mov	DWORD PTR [rbp-20], edx
	mov	DWORD PTR [rbp-24], ecx
#   используем расширения современных intel процессоров.
#   source            = r8
#   current_character = r9
#   was_separator     = r10
#   begins_index      = r11
#   счётчик - индекс ещё не заполненной ячейки в массиве индексов начал слов
#   begins_index = 0;
    mov r11, 0
#   флаг "предыдущим символом был разделитель"
#   == true, так как начало строки - тоже разделитель
#   was_separator = 1;
	mov r10, 1
#   индекс текущего символа
#   current_character = 0;
	mov r9, 0
#   инициализируем source
	mov r8, QWORD PTR [rbp-8]
begin_parse_loop:
#   if (source[current_character] == ' '){
	mov al, [r8+r9]
	cmp al, ' '
	jne after_spase_check
#       was_separator = 1;
	mov r10b, 1
#       ++current_character;
	inc r9
#       goto begin_parse_loop;
	jmp begin_parse_loop
#   }
after_spase_check:
#   if (source[current_character] == '\0'){
	cmp al, 0
	jne after_null_check
#       //последняя ячейка содержит индекс конца строки
#       begin_word_index[begins_index] = current_character;
	mov [begin_word_index+r11], r9b
#       goto end_parse_loop;
	jmp end_parse_loop
#   }
after_null_check:
#   // если любой другой символ, не ␠, не ␀
#   if (was_separator){
	cmp r10b, 1
	jne after_was_separator_check
#       begin_word_index[begins_index] = current_character;
	mov [(begin_word_index)+r11], r9b
#       ++begins_index;
	inc r11
#       was_separator = 0;
	mov r10, 0
#       ++current_character;
	inc r9
#       goto begin_parse_loop;
	jmp begin_parse_loop
#   }
after_was_separator_check:
#   если символ не после пробела и не в начале строки
#   просто переходим к следующему
#   ++current_character;
	inc r9
#   goto begin_parse_loop;
	jmp begin_parse_loop
end_parse_loop:
#   только begin_word_index, begins_index и параметры функции важны после этой точки
#   на данном этапе в массиве begin_word_index хранятся начала слов в количестве begins_index;
#   теперь копируем в назначение из источника в назначение.
#              ↓word1           ↓word2
#   =====-====-=========-======-========-=======-=======0
#   [от нулевого до индекса первого меняемого слова)
#              ↓                ↓
#   ≈≈≈≈≈~≈≈≈≈~=========-======-========-=======-=======0
#   [от индекса второго меняемого слова до индекс второго меняемого слова+1)
#              ↓                ↓
#   ≈≈≈≈≈~≈≈≈≈~=========-======-≈≈≈≈≈≈≈≈~=======-=======0
#   [от индекса первого меняемого + 1 до индека второго меняемого)
#              ↓                ↓
#   ≈≈≈≈≈~≈≈≈≈~=========-≈≈≈≈≈≈~≈≈≈≈≈≈≈≈~=======-=======0
#   [от индекcа первого меняемого до индекса индекса первого меняемого + 1)
#              ↓                ↓
#   ≈≈≈≈≈~≈≈≈≈~≈≈≈≈≈≈≈≈≈~≈≈≈≈≈≈~≈≈≈≈≈≈≈≈~=======-=======0
#   [от индекса второго меняемого до индекса конца begins_index]
#              ↓                ↓
#   ≈≈≈≈≈~≈≈≈≈~≈≈≈≈≈≈≈≈≈~≈≈≈≈≈≈~≈≈≈≈≈≈≈≈~≈≈≈≈≈≈≈~≈≈≈≈≈≈≈0
#   храним локальные переменные в регистрах
#   отдельно напомню, что где в коде ниже:
#   word1             = r8
#   word2             = r9
#   begins_index      = r11
#   sites_i           = r12 
#   src_i             = r13
#   dest_i            = r14
#   destination       = rdi
#   source            = rsi
#   инициализация регистров
#   word1             = r8           int
    xor r8, r8
    mov r8d, DWORD PTR [rbp-20]
#   word2             = r9           int
    xor r9, r9
    mov r9d, DWORD PTR [rbp-24]
#   только тут мы можем провалидировать входные параметры.
#   if (word1 > word2) { // word1 должен быть меньше word2
    cmp r8, r9
    jle after_word1_word2_compare
#       unsigned int tmp = word1;
#       word1 = word2;
#       word2 = tmp;
    xchg r8, r9
#   }
after_word1_word2_compare:
#   begins_index      = r11          char
#   сохраняет значение из кода выше
#   индекс, в какой участок из вышезаданных мы копируем.
#   sites_i           = r12          char
    mov r12, 0
#   src_i             = r13          char
    mov r13, 0
#   индекс на место, куда будем копировать в назначении, сквозной.
#   dest_i            = r14          char
    mov r14, 0
#   destination       = rdi          char *
    mov	rdi, QWORD PTR [rbp-16]
#   source            = rsi          char *
    mov	rsi, QWORD PTR [rbp-8]
#   if (word2 < begins_index) { // если просят поменять существующие слова
    cmp r9, r11
    jge copy_char_without_swap_word
#   формируем массив нижних границ копирования, включая копируемый символ.
#   from_indexes[0] = 0;
    mov BYTE PTR[from_indexes], 0
#   from_indexes[1] = begin_word_index[word2];
    mov al, BYTE PTR[begin_word_index+r9]
    mov BYTE PTR[from_indexes+1], al
#   from_indexes[2] = begin_word_index[word1+1];
    mov al, BYTE PTR[begin_word_index+r8+1]
    mov BYTE PTR[from_indexes+2], al
#   from_indexes[3] = begin_word_index[word1];
    mov al, BYTE PTR[begin_word_index+r8]
    mov BYTE PTR[from_indexes+3], al
#   from_indexes[4] = begin_word_index[word2+1];
    mov al, BYTE PTR[begin_word_index+r9+1]
    mov BYTE PTR[from_indexes+4], al
#   массив верхних границ копирования, не включая этот символ.
#   to_indexes[0] = begin_word_index[word1];
    mov al, BYTE PTR[begin_word_index+r8]
    mov BYTE PTR[to_indexes], al
#   to_indexes[1] = begin_word_index[word2+1];
    mov al, BYTE PTR[begin_word_index+r9+1]
    mov BYTE PTR[to_indexes+1], al
#   to_indexes[2] = begin_word_index[word2];
    mov al, BYTE PTR[begin_word_index+r9]
    mov BYTE PTR[to_indexes+2], al
#   to_indexes[3] = begin_word_index[word1+1];
    mov al, BYTE PTR[begin_word_index+r8+1]
    mov BYTE PTR[to_indexes+3], al
#   to_indexes[4] = begin_word_index[begins_index];
    mov al, BYTE PTR[begin_word_index+r11]
    mov BYTE PTR[to_indexes+4], al
begin_copy_sites_loop:
#   src_i = from_indexes[sites_i];
    mov r13b, BYTE PTR[from_indexes+r12]
begin_copy_char_loop:
#   if (src_i < to_indexes[sites_i]){
    cmp r13b, [to_indexes+r12]
    jge after_upper_bound_check
#       destination[dest_i] = source[src_i];
    mov al, BYTE PTR[rsi+r13]
    mov BYTE PTR[rdi+r14], al
#       ++dest_i;
    inc r14
#       ++src_i;
    inc r13
#       goto begin_copy_char_loop;
    jmp begin_copy_char_loop
#   }
after_upper_bound_check:
#   if(sites_i < 5){
    cmp r12b, 5
    jge end_all_copy
#       ++sites_i;
    inc r12
#       goto begin_copy_sites_loop;
    jmp begin_copy_sites_loop
#   }
#   end_copy_sites_loop
#   } else {
#   если надо поменять слова за пределами существующих, то просто копируем из источника в назначение.
copy_char_without_swap_word:
begin_memcpy_loop:
#       char letter = source[src_i];
    mov al, [rsi+r13]
#       if (letter != 0) {
    cmp al, 0
    je end_all_copy
#           destination[dest_i] = letter;
    mov BYTE PTR[rdi+r14], al
#           ++src_i;
    inc r13
#           ++dest_i;
    inc r14
#           goto begin_memcpy_loop;
    jmp begin_memcpy_loop
#       }
#   }
end_all_copy:
#   destination[dest_i] = '\0';
    mov BYTE PTR[rdi+r14], 0
    nop
#   printf("%s", dest);
#   printf вызывается __fastcall'ом - функция очень старая, старше unix.
    mov	rsi, rdi
    lea rdi, printf_format_string
    mov	rax, 0
    call	printf
#   восстанавливаем состояние стека.
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	swap_word, .-swap_word
	.ident	"Oleg Schwann, novice in assembler, with assistance GCC 5."
	.section	.note.GNU-stack,"",@progbits
