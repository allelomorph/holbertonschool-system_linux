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
	;; 	while ((*s1 || *s2) && diff == 0)
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
	jmp     .s1_or_s2_test		;
.c1_ternary:				;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		; deref s1
	cmp     al, 64			; *s1 >= 'A'
	jle     .c1_ternary_false	;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		; deref s1
	cmp     al, 90			; *s1 >= 'Z'
	jg      .c1_ternary_false	;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		; deref s1
	add     eax, 32			; *s1 + ' '
	jmp     .c1_ternary_true	;
.c1_ternary_false:			;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		; c1 = *s1
.c1_ternary_true:			;
	mov     BYTE [rbp - 5], al	; c1 = *s1 + ' '
	mov     rax, QWORD [rbp - 32]	; begin: c2 ternary
	movzx   eax, BYTE [rax]		; deref s2
	cmp     al, 64			; *s2 >= 'A'
	jle     .c2_ternary_false	;
	mov     rax, QWORD [rbp - 32]	;
	movzx   eax, BYTE [rax]		; deref s2
	cmp     al, 90			; *s2 >= 'Z'
	jg      .c2_ternary_false	;
	mov     rax, QWORD [rbp - 32]	;
	movzx   eax, BYTE [rax]		;
	add     eax, 32			;
	jmp     .find_diff		;
.c2_ternary_false:			;
	mov     rax, QWORD [rbp - 32]	;
	movzx   eax, BYTE [rax]		;
.find_diff:				;
	mov     BYTE [rbp - 6], al	;
	movsx   edx, BYTE [rbp - 5]	;
	movsx   eax, BYTE [rbp - 6]	;
	sub     edx, eax		;
	mov     eax, edx		;
	mov     DWORD [rbp - 4], eax	;
	add     QWORD [rbp - 24], 1	; s1++
	add     QWORD [rbp - 32], 1	; s2++
.s1_or_s2_test:				;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		;
	test    al, al			;
	jne     .diff_test		;
	mov     rax, QWORD [rbp - 32]	;
	movzx   eax, BYTE [rax]		;
	test    al, al			;
	je      .return_diff		;
.diff_test:
	cmp     DWORD [rbp - 4], 0	; diff == 0?
	je      .c1_ternary		;
.return_diff:				;
	mov     eax, DWORD [rbp - 4]	;
	pop     rbp			; epilogue
	ret				;
