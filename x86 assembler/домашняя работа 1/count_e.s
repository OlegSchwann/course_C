	.file	"count_e.s"
	.intel_syntax noprefix
	.text
	.globl	count_e
	.type	count_e, @function
.LFB0:
count_e:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	QWORD PTR [rbp-8], rdi
	mov	QWORD PTR [rbp-16], rsi
#   unsigned int word_i = 0; // r15
    xor r15, r15
#   unsigned int char_i = 0; // r14
    xor r14, r14
#   char was_delimiter  = 1; // r13
    mov r13, 1
#   const char *string       // r12
    mov	r12, QWORD PTR [rbp-8]
#   char *arr_e              // r11
    mov	r11, QWORD PTR [rbp-16]
#   обнуляем регистр, в который читаем символы
    xor rax, rax
begin_cycle:
#   while (string[char_i] != '\0') {
    mov al, [r12 + r14]
    cmp al, 0
    je end_cycle
#       if (string[char_i] == ' ') {
    cmp al, ' '
    jne else_spase_check
#           if (was_delimiter == 0) {
    cmp r13b, 0
    jne to_cycle_increment
#               ++word_i; значит переходим к следующему слову.
    inc r15
#               was_delimiter = 1;
    mov r13b, 1
#           }
    jne to_cycle_increment
else_spase_check:
#       } else {
#           was_delimiter = 0;
    mov r13b, 0
#       }
after_spase_check:
#       if (string[char_i] == 'e') { #если буква
    cmp al, 'e'
    jne to_cycle_increment
#           ++arr_e[word_i];
    inc BYTE PTR [r11+r15]
#       }
to_cycle_increment:
#       ++char_i;
    inc r14
#   }
    jmp begin_cycle
end_cycle:
#   if (was_delimetr == 0){
    cmp r13b, 0
    jne end_processing
#       ++word_i;
    inc r15
#   } если заканчивается на пробел, то уже не надо увеличивать количество слов
end_processing:
#e   return word_i;
    mov rax, r15
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	count_e, .-count_e
	.ident	"Oleg Schwann with help of GCC & GDB."
	.section	.note.GNU-stack,"",@progbits
