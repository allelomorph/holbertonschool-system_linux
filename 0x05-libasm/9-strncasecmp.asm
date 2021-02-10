BITS 64

global asm_strncasecmp		; export `asm_strncasecmp` function
section .text

	;; /* originally implemented with a helper function that cloned tolower(). */
	;; /* but `call` instruction is prohibited by project rules, so switched to ternary */
	;;
	;; int asm_strncasecmp(const char *s1, const char *s2, size_t n)
	;; {
	;;	size_t i = 0;
	;; 	int diff = 0;
	;; 	char c1, c2;
	;;
	;; 	while ((*s1 || *s2) && diff == 0 && i < n)
	;; 	{
	;; 		c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + ' ' : *s1;
	;; 		c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + ' ' : *s2;
	;; 		diff = c1 - c2;
	;; 		s1++;
	;; 		s2++;
	;; 		i++;
	;; 	}
	;;
	;; 	/* 9-main is expecting difference in ASCII values, not 0/1/-1 implementation */
	;; 	return (diff);
	;; }

asm_strncasecmp:
	push    rbp			; prologue
	mov     rbp, rsp		;
	mov     QWORD [rbp - 24], rdi	; reserve space on stack for s1 from rdi
	mov     QWORD [rbp - 32], rsi	; reserve space on stack for s2 from rsi
	mov     QWORD [rbp - 40], rdx	; reserve space on stack for n from rdx
	mov     QWORD [rbp - 8], 0	; reserve space on stack for i
	mov     DWORD [rbp - 12], 0	; reserve space on stack for diff (and c1, c2)
	jmp     .s1_or_s2_test		;
.c1_ternary:				;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		; deref s1
	cmp     al, 64			; *s1 >= 'A'
	jle     .c1_ternary_false	;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		; deref s1
	cmp     al, 90			; *s1 <= 'Z'
	jg      .c1_ternary_false	;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		; deref s1
	add     eax, 32			; *s1 + ' '
	jmp     .c1_ternary_true	;
.c1_ternary_false:			;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		; c1 = *s1
.c1_ternary_true:			;
	mov     BYTE [rbp - 13], al	; c1 = *s1 + ' '
	mov     rax, QWORD [rbp - 32]	; begin: c2 ternary
	movzx   eax, BYTE [rax]		; deref s2
	cmp     al, 64			; *s2 >= 'A'
	jle     .c2_ternary_false	;
	mov     rax, QWORD [rbp - 32]	;
	movzx   eax, BYTE [rax]		; deref s2
	cmp     al, 90			; *s2 <= 'Z'
	jg      .c2_ternary_false	;
	mov     rax, QWORD [rbp - 32]	;
	movzx   eax, BYTE [rax]		; deref s2
	add     eax, 32			; *s2 + ' '
	jmp     .find_diff		;
.c2_ternary_false:			;
	mov     rax, QWORD [rbp - 32]	;
	movzx   eax, BYTE [rax]		; c2 = *s2
.find_diff:				;
	mov     BYTE [rbp - 14], al	; c2 = *s2 + ' '
	movsx   edx, BYTE [rbp - 13]	;
	movsx   eax, BYTE [rbp - 14]	;
	sub     edx, eax		; c1 - c2
	mov     eax, edx		;
	mov     DWORD [rbp - 12], eax	; diff = result of c1 - c2
	add     QWORD [rbp - 24], 1	; s1++
	add     QWORD [rbp - 32], 1	; s2++
	add     QWORD [rbp - 8], 1	; i++
.s1_or_s2_test:				;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		;
	test    al, al			;
	jne     .diff_i_tests		;
	mov     rax, QWORD [rbp - 32]	;
	movzx   eax, BYTE [rax]		;
	test    al, al			;
	je      .return_diff		;
.diff_i_tests:				;
        cmp     DWORD [rbp - 12], 0	; diff == 0?
	jne     .return_diff		;
	mov     rax, QWORD [rbp - 8]	;
	cmp     rax, QWORD [rbp - 40]	; i < n?
	jb      .c1_ternary		;
.return_diff:				;
	mov     eax, DWORD [rbp - 12]	;
	pop     rbp			; epilogue
	ret				;
