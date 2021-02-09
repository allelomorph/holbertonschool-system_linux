BITS 64

global asm_strcmp			; EXPORT `asm_strcmp` function
section .text

	;; int asm_strcmp(const char *s1, const char *s2)
	;; {
	;; 	while (*s1 && *s2 && *s1 == *s2)
	;;	{
	;; 		s1++;
	;; 		s2++;
	;; 	}
	;;
	;; 	if (*s1 > *s2)
	;; 		return (1);
	;; 	if (*s1 < *s2)
	;; 		return (-1);
	;;	return (0);
	;; }

asm_strcmp:
	push    rbp			; prologue
	mov     rbp, rsp		;
	mov     QWORD [rbp - 8], rdi	; reserve space for s1 on stack
	mov     QWORD [rbp - 16], rsi	; reserve space for s2 on stack
	jmp     .s1_s2_loop_tests	;
.inc_s1_s2:				;
	add     QWORD [rbp - 8], 1	; s1++
	add     QWORD [rbp - 16], 1	; s2++
.s1_s2_loop_tests:			;
	mov     rax, QWORD [rbp - 8]	;
	movzx   eax, BYTE [rax]		;
	test    al, al			;
	je      .test_s1_gt_s2		;
	mov     rax, QWORD [rbp - 16]	;
	movzx   eax, BYTE [rax]		;
	test    al, al			;
	je      .test_s1_gt_s2	 	;
	mov     rax, QWORD [rbp - 8]	;
	movzx   edx, BYTE [rax]		;
	mov     rax, QWORD [rbp - 16]	;
	movzx   eax, BYTE [rax]		;
	cmp     dl, al			;
	je      .inc_s1_s2		;
.test_s1_gt_s2:				;
	mov     rax, QWORD [rbp - 8]	;
	movzx   edx, BYTE [rax]		;
	mov     rax, QWORD [rbp - 16]	;
	movzx   eax, BYTE [rax]		;
	cmp     dl, al			;
	jle     .test_s1_lt_s2		;
	mov     eax, 1			;
	jmp     .return			;
.test_s1_lt_s2:				;
	mov     rax, QWORD [rbp - 8]	;
	movzx   edx, BYTE [rax]		;
	mov     rax, QWORD [rbp - 16]	;
	movzx   eax, BYTE [rax]		;
	cmp     dl, al			;
	jge     .return_0		;
	mov     eax, -1			;
	jmp     .return			;
.return_0:				;
	mov     eax, 0			;
.return:				;
	pop     rbp			; epilogue
	ret				;
