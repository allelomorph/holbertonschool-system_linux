BITS 64

global asm_strspn		; export `asm_strspn` function
section .text

	;; size_t asm_strspn(const char *s, const char *accept)
	;; {
	;;	size_t i = 0;
	;;	char *temp = NULL;
	;;
	;;	while (*s)
	;; 	{
	;;		temp = (char *)accept;
	;; 		while (*temp && *temp != *s)
	;; 			temp++;
	;; 		if (!(*temp))
	;; 			break;
	;; 		i++;
	;; 		s++;
	;; 	}
	;;
	;; 	return (i);
	;; }

asm_strspn:
	push    rbp			; prologue
	mov     rbp, rsp		;
	mov     QWORD [rbp - 24], rdi 	; s comes in as rdi, reserve space in stack memory
	mov     QWORD [rbp - 32], rsi	; accept comes in as rsi, reserve space in stack memory
	mov     QWORD [rbp - 8], 0	; reserve stack memory for i = 0
	mov     QWORD [rbp - 16], 0	; reserve stack memory for temp = NULL
	jmp     .s_loop			;
.cast_accept_to_temp:			;
	mov     rax, QWORD [rbp - 32]	;
	mov     QWORD [rbp - 16], rax	;
	jmp     .temp_loop		;
.inc_temp:				;
	add     QWORD [rbp - 16], 1	;
.temp_loop:				;
	mov     rax, QWORD [rbp - 16]	;
	movzx   eax, BYTE [rax]		; deref temp
	test    al, al			; temp == NULL?
	je      .test_temp_on_loop_exit	;
	mov     rax, QWORD [rbp - 16]	;
	movzx   edx, BYTE [rax]		;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		;
	cmp     dl, al			;
	jne     .inc_temp		;
.test_temp_on_loop_exit:		;
	mov     rax, QWORD [rbp - 16]	;
	movzx   eax, BYTE [rax]		; deref temp
	test    al, al			; temp == NULL?
	jne     .inc_i_s		;
	jmp     .return_i		;
.inc_i_s:				;
	add     QWORD [rbp - 8], 1	; i++
	add     QWORD [rbp - 24], 1	; s++
.s_loop:				;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		; deref s
	test    al, al			; s == NULL?
	jne     .cast_accept_to_temp	;
.return_i:				;
	mov     rax, QWORD [rbp - 8]	;
	pop     rbp			; epilogue
	ret				;
