BITS 64

global asm_strpbrk		; export `asm_strpbrk` function
section .text

	;; char *asm_strpbrk(const char *s, const char *accept)
	;; {
	;; 	char *temp = NULL;
	;;
	;;	while (*s)
	;; 	{
	;; 		temp = (char *)accept;
	;; 		while (*temp && *temp != *s)
	;; 			temp++;
	;; 		if (*temp)
	;; 			break;
	;; 		s++;
	;; 	}
	;;
	;; 	if (*temp)
	;; 		return ((char *)s);
	;; 	else
	;; 		return (NULL);
	;; }

asm_strpbrk:
	push    rbp			; prologue
	mov     rbp, rsp		;
	mov     QWORD [rbp - 24], rdi 	; s comes in as rdi, reserve space in stack memory
	mov     QWORD [rbp - 32], rsi 	; accept comes in as rsi, reserve space in stack memory
	mov     QWORD [rbp - 8], 0	; reserve stack memory for temp = NULL
	jmp     .s_loop			;
.cast_accept_to_temp:			;
	mov     rax, QWORD [rbp - 32]	;
	mov     QWORD [rbp - 8], rax	;
	jmp     .temp_loop_test		;
.inc_temp:				;
	add     QWORD [rbp - 8], 1	;
.temp_loop_test:			;
	mov     rax, QWORD [rbp - 8]	;
	movzx   eax, BYTE [rax]		;
	test    al, al			; *temp == '\0'?
	je      .test_temp_after_loop	;
	mov     rax, QWORD [rbp - 8]	;
	movzx   edx, BYTE [rax]		;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		;
	cmp     dl, al			;
	jne     .inc_temp		;
.test_temp_after_loop:			;
	mov     rax, QWORD [rbp - 8]	;
	movzx   eax, BYTE [rax]		;
	test    al, al			;
	je      .inc_s			;
	jmp     .select_return		;
.inc_s:					;
	add     QWORD [rbp - 24], 1	;
.s_loop:				;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		;
	test    al, al			;
	jne     .cast_accept_to_temp	;
.select_return:				;
	mov     rax, QWORD [rbp - 8]	;
	movzx   eax, BYTE [rax]		;
	test    al, al			; *temp == '\0'?
	je      .return_null		;
	mov     rax, QWORD [rbp - 24]	;
	jmp     .return_s		;
.return_null:				;
	mov     eax, 0			;
.return_s:				;
	pop     rbp			; epilogue
	ret				;
