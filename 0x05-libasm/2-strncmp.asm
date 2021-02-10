BITS 64

global asm_strncmp			; EXPORT `asm_strncmp` function
section .text

	;; int asm_strncmp(const char *s1, const char *s2, size_t n)
	;; {
	;;	size_t i = 0;
	;; 	int diff = 0;
	;;
	;;	while (*s1 && *s2 && diff == 0 && i < n)
	;; 	{
	;; 		diff = *s1 - *s2;
	;; 	        s1++;
	;; 		s2++;
	;; 		i++;
	;;	}
	;;
	;; 	if (diff > 0)
	;; 		return (1);
	;; 	if (diff < 0)
	;; 		return (-1);
	;; 	return (0);
	;; }

asm_strncmp:
	push    rbp			; prologue
	mov     rbp, rsp		;
	mov     QWORD [rbp - 24], rdi	; reserve space on stack for s1
	mov     QWORD [rbp - 32], rsi	; reserve space on stack for s2
	mov     QWORD [rbp - 40], rdx	; reserve space on stack for n
	mov     QWORD [rbp - 8], 0	; reserve space on stack for i == 0
	mov     DWORD [rbp - 12], 0	; reserve space on stack for diff == 0
	jmp     .s1_or_s2_test		;
.s1_s2_loop:				;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		; deref s1
	movsx   edx, al			;
	mov     rax, QWORD [rbp - 32]	;
	movzx   eax, BYTE [rax]		; deref s2
	movsx   eax, al			;
	sub     edx, eax		; *s1 - *s2
	mov     eax, edx		;
	mov     DWORD [rbp - 12], eax	;
	add     QWORD [rbp - 24], 1	; s1++
	add     QWORD [rbp - 32], 1	; s2++
	add     QWORD [rbp - 8], 1	; i++
.s1_or_s2_test:				;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		; deref s1
	test    al, al			; *s1 == '\0'?
	jne     .diff_i_tests		;
	mov     rax, QWORD [rbp - 32]	;
	movzx   eax, BYTE [rax]		; deref s2
	test    al, al			; *s2 == '\0'?
	je      .diff_gt_0		;
.diff_i_tests:				;
	cmp     DWORD [rbp - 12], 0	; diff == 0?
	jne     .diff_gt_0		;
	mov     rax, QWORD [rbp - 8]	;
	cmp     rax, QWORD [rbp - 40]	; i < n?
	jb      .s1_s2_loop		;
.diff_gt_0:				;
	cmp     DWORD [rbp - 12], 0	;
	jle     .diff_lt_0		;
	mov     eax, 1			;
	jmp     .return			;
.diff_lt_0:				;
	cmp     DWORD [rbp - 12], 0	;
	jns     .return_0		;
	mov     eax, -1			;
	jmp     .return			;
.return_0:				;
	mov     eax, 0			;
.return:				;
	pop     rbp			; epilogue
	ret				;
