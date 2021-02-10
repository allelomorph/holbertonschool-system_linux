BITS 64

global asm_strcasecmp		; export `asm_strcasecmp` function
section .text

	;; /* originally implemented with a helper function that cloned tolower(). */
	;; /* but `call` instruction is prohibited by project rules, so switched to ternary */
	;;
	;; int asm_strcasecmp(const char *s1, const char *s2)
	;; {
	;; 	char c1, c2;
	;; 	int diff = 0;
	;;
	;; 	while (*s1 && *s2 && diff == 0)
	;; 	{
	;; 		c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + ' ' : *s1;
	;; 		c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + ' ' : *s2;
	;; 		diff = c1 - c2;
	;; 		s1++;
	;; 		s2++;
	;; 	}
	;;
	;; 	/* 8-main is expecting difference in ASCII values, not 0/1/-1 implementation */
	;; 	return (diff);
	;; }

asm_strcasecmp:
	push    rbp			; prologue
	mov     rbp, rsp		;
	mov     QWORD [rbp - 24], rdi	; reserve space on stack for s1 from rdi
	mov     QWORD [rbp - 32], rsi	; reserve space on stack for s2 from rsi
	mov     DWORD [rbp - 4], 0	; reserve space on stack for diff = 0 (and c1, c2)
	jmp     .s1_s2_loop_tests	;
.c1_ternary:				;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		;
	cmp     al, 64			;
	jle     .L3			;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		;
	cmp     al, 90			;
	jg      .L3			;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		;
	add     eax, 32			;
	jmp     .L4			;
.L3:					;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		;
.L4:					;
	mov     BYTE [rbp - 5], al	;
	mov     rax, QWORD [rbp - 32]	;
	movzx   eax, BYTE [rax]		;
	cmp     al, 64			;
	jle     .L5			;
	mov     rax, QWORD [rbp - 32]	;
	movzx   eax, BYTE [rax]		;
	cmp     al, 90			;
	jg      .L5			;
	mov     rax, QWORD [rbp - 32]	;
	movzx   eax, BYTE [rax]		;
	add     eax, 32			;
	jmp     .L6			;
.L5:					;
	mov     rax, QWORD [rbp - 32]	;
	movzx   eax, BYTE [rax]		;
.L6:					;
	mov     BYTE [rbp - 6], al	;
	movsx   edx, BYTE [rbp - 5]	;
	movsx   eax, BYTE [rbp - 6]	;
	sub     edx, eax		;
	mov     eax, edx		;
	mov     DWORD [rbp - 4], eax	;
	add     QWORD [rbp - 24], 1	;
	add     QWORD [rbp - 32], 1	;
.s1_s2_loop_tests:			;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		;
	test    al, al			;
	je      .L7			;
	mov     rax, QWORD [rbp - 32]	;
	movzx   eax, BYTE [rax]		;
	test    al, al			;
	je      .L7			;
	cmp     DWORD [rbp - 4], 0	;
	je      .c1_ternary		;
.L7:					;
	mov     eax, DWORD [rbp - 4]	;
	pop     rbp			; epilogue
	ret				;
