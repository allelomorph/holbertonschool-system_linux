BITS 64

global asm_strchr			; EXPORT `asm_strchr` function
section .text

	;; char *asm_strchr(const char *s, int c)
	;; {
	;; 	char *temp = (char *)s;
	;;
	;;	while (*temp)
	;; 	{
	;;	 	if (*temp == c)
	;; 			return (temp);
	;; 		temp++;
	;;	}
	;;
	;; 	return (NULL);
	;; }

asm_strchr:
	push    rbp			; prologue
	mov     rbp, rsp		;
	mov     QWORD [rbp - 24], rdi	; reserve space on stack for s
	mov     DWORD [rbp - 28], esi	; reserve space on stack for c
	mov     rax, QWORD [rbp - 24]	; cast s
	mov     QWORD [rbp - 8], rax	; reserve space on stack for temp
	jmp     .temp_loop_test		;
.temp_loop:				;
	mov     rax, QWORD [rbp - 8]	;
	movzx   eax, BYTE [rax]		;
	movsx   eax, al			;
	cmp     eax, DWORD [rbp - 28]	;
	jne     .inc_temp		;
	mov     rax, QWORD [rbp - 8]	;
	jmp     .return			;
.inc_temp:				;
	add     QWORD [rbp - 8], 1	; temp++
.temp_loop_test:			;
	mov     rax, QWORD [rbp - 8]	;
	movzx   eax, BYTE [rax]		;
	test    al, al			; *temp == '\0'?
	jne     .temp_loop		;
	mov     eax, 0			;
.return:				;
	pop     rbp			; epilogue
	ret				;
