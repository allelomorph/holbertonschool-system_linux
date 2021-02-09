BITS 64

global asm_tolower		; export `asm_tolower` function
global asm_strncasecmp		; export `asm_strncasecmp` function
section .text

	;; int asm_tolower(int c)
	;; {
	;; 	if (c >= 'A' && c <= 'Z')
	;; 		return (c + ' ');
	;; 	else
	;; 		return (c);
	;; }
	;;
	;; int asm_strncasecmp(const char *s1, const char *s2, size_t n)
	;; {
	;;	size_t i = 0;
	;; 	int diff = 0;
	;;
	;;	while (*s1 && *s2 && diff == 0 && i < n)
	;; 	{
	;;		diff = asm_tolower(*s1) - asm_tolower(*s2) ;
	;;		s1++;
	;;		s2++;
	;;		i++;
	;;	}
	;;
	;;	/* 9-main is expecting difference in ASCII values, not 0/1/-1 implementation */
	;;	return (diff);
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
asm_strncasecmp:			;
	push    rbp			; prologue
	mov     rbp, rsp		;
	push    rbx			; rbx is return value of asm_tolower?
	sub     rsp, 40			; reserve 5 8-byte spaces on stack
	mov     QWORD [rbp - 32], rdi 	; store s1 from rdi on stack
	mov     QWORD [rbp - 40], rsi 	; store s2 from rsi on stack
	mov     QWORD [rbp - 48], rdx 	; store n from rdx on stack
	mov     QWORD [rbp - 16], 0 	; store i on stack
	mov     DWORD [rbp - 20], 0 	; store diff on stack
	jmp     .s1_s2_loop_tests      	;
.s1_s2_loop:				;
        mov     rax, QWORD [rbp - 32] 	; deref s1
	movzx   eax, BYTE [rax]		;
	movsx   eax, al			;
	mov     edi, eax		;
	call    asm_tolower		;
	mov     ebx, eax		; store first asm_tolower return in ebx
	mov     rax, QWORD [rbp - 40] 	; deref s2
	movzx   eax, BYTE [rax]		;
	movsx   eax, al			;
	mov     edi, eax		;
	call    asm_tolower		;
	sub     ebx, eax		; subtract second as_tolower return
	mov     eax, ebx		;
	mov     DWORD [rbp - 20], eax 	; store result in diff
	add     QWORD [rbp - 32], 1	; s1++
	add     QWORD [rbp - 40], 1	; s2++
	add     QWORD [rbp - 16], 1	; i++
.s1_s2_loop_tests:		      	;
	mov     rax, QWORD [rbp - 32]	;
	movzx   eax, BYTE [rax]		; deref s1
	test    al, al			; test s1 not 0
	je      .return_diff		;
	mov     rax, QWORD [rbp - 40]	;
	movzx   eax, BYTE [rax]		; deref s2
	test    al, al			; test s2 not 0
	je      .return_diff		;
	cmp     DWORD [rbp - 20], 0	;
	jne     .return_diff		;
	mov     rax, QWORD [rbp - 16]	;
	cmp     rax, QWORD [rbp - 48]	; test i < n
	jb      .s1_s2_loop		;
.return_diff:				;
	mov     eax, DWORD [rbp - 20]	;
	add     rsp, 40			;
	pop     rbx			;
	pop     rbp			;
	ret				;
