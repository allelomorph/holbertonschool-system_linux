BITS 64

global asm_tolower		; export `asm_tolower` function
global asm_strcasecmp		; export `asm_strcasecmp` function
section .text

	;; int asm_tolower(int c)
	;; {
	;; 	if (c >= 'A' && c <= 'Z')
	;; 		return (c + ' ');
	;; 	else
	;; 		return (c);
	;; }
	;;
	;; int asm_strcasecmp(const char *s1, const char *s2)
	;; {
	;; 	while (*s1 && *s2 && asm_tolower(*s1) == asm_tolower(*s2))
	;; 	{
	;; 		s1++;
	;; 		s2++;
	;;	}
	;;
	;; 	/* 8-main is expecting difference in ASCII values, not 0/1/-1 implementation */
	;; 	return (asm_tolower(*s1) - asm_tolower(*s2));
	;; }

asm_tolower:
	push    rbp			; prologue
	mov     rbp, rsp		;
	mov     DWORD [rbp - 4], edi 	; reserve 4 bytes on stack for int c in edi
	cmp     DWORD [rbp - 4], 64	; compare c to 'A'
	jle     .return_c		;
	cmp     DWORD [rbp - 4], 90 	; compare c to 'Z'
	jg      .return_c		;
	mov     eax, DWORD [rbp - 4]	;
	add     eax, 32			; c + ' '
	jmp     .return_c_lower		;
.return_c:				;
	mov     eax, DWORD [rbp - 4]	; return c as is
.return_c_lower:			;
	pop     rbp			; return lowercase c (why rbp and not eax?)
	ret				;
					;;
asm_strcasecmp:				;
	push    rbp			; prologue
	mov     rbp, rsp		;
	push    rbx			; rbx is return value of asm_tolower?
	sub     rsp, 16			; reserve two more 8 byte spaces on stack
	mov     QWORD [rbp - 16], rdi 	; store rdi on stack
	mov     QWORD [rbp - 24], rsi 	; store rsi on stack
	jmp     .s1_s2_loop_tests      	;
.inc_s1_s2:				;
	add     QWORD [rbp - 16], 1   	; s1++
	add     QWORD [rbp - 24], 1   	; s2++
.s1_s2_loop_tests:		      	;
	mov     rax, QWORD [rbp - 16] 	;
	movzx   eax, BYTE [rax]	      	;
	test    al, al		      	;
	je      .find_lowercase_diff	;
	mov     rax, QWORD [rbp - 24] 	;
	movzx   eax, BYTE [rax]	      	;
	test    al, al		      	;
	je      .find_lowercase_diff	;
	mov     rax, QWORD [rbp - 16]	;
	movzx   eax, BYTE [rax]		;
	movsx   eax, al			;
	mov     edi, eax		;
	call    asm_tolower		;
	mov     ebx, eax		;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		;
	movsx   eax, al			;
	mov     edi, eax		;
	call    asm_tolower		;
	cmp     ebx, eax		;
	je      .inc_s1_s2		;
.find_lowercase_diff:			;
	mov     rax, QWORD [rbp - 16]	; begin deref s1 into 1 byte int
	movzx   eax, BYTE [rax]		;
	movsx   eax, al			; end deref s1 into 1 byte int
	mov     edi, eax		; asm_tolower uses edi
	call    asm_tolower		;
	mov     ebx, eax		; store first return of asm_tolower in ebx
	mov     rax, QWORD [rbp - 24]	; begin deref s1 into 1 byte int
	movzx   eax, BYTE [rax]		;
	movsx   eax, al			; end deref s1 into 1 byte int
	mov     edi, eax		; asm_tolower uses edi
	call    asm_tolower		;
	sub     ebx, eax		; difference in returns
	mov     eax, ebx		;
	add     rsp, 16			; returning caclulated difference in lowercase ASCII
	pop     rbx			;
	pop     rbp			;
	ret				;
