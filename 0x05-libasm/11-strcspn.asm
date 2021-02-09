BITS 64

global asm_strcspn		; export `asm_strcspn` function
section .text

	;; size_t asm_strcspn(const char *s, const char *reject)
	;; {
	;;	size_t i = 0;
	;;	char *temp = NULL;
	;;
	;;	while (*s)
	;; 	{
	;;		temp = (char *)reject;
	;; 		while (*temp && *temp != *s)
	;; 			temp++;
	;; 		if (*temp)
	;; 			break;
	;; 		i++;
	;; 		s++;
	;; 	}
	;;
	;; 	return (i);
	;; }

asm_strcspn:
	push    rbp			; prologue
	mov     rbp, rsp		;
	mov     QWORD [rbp - 24], rdi 	; s comes in as rdi, reserve space in stack memory
	mov     QWORD [rbp - 32], rsi	; reject comes in as rsi, reserve space in stack memory
	mov     QWORD [rbp - 8], 0	; reserve stack memory for i = 0
	mov     QWORD [rbp - 16], 0	; reserve stack memory for temp = NULL
	jmp     .s_loop			;
.cast_reject_to_temp:			;
	mov     rax, QWORD [rbp - 32]	;
	mov     QWORD [rbp - 16], rax	;
	jmp     .temp_loop		;
.inc_temp:				;
	add     QWORD [rbp - 16], 1	;
.temp_loop:				;
	mov     rax, QWORD [rbp - 16]	;
	movzx   eax, BYTE [rax]		; deref temp
	test    al, al			; temp == NULL?
	je      .test_temp_on_loop_exit	; difference from asm_strspn: je here instead of jne
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
	je    	.inc_i_s		;
	jmp     .return_i		;
.inc_i_s:				;
	add     QWORD [rbp - 8], 1	; i++
	add     QWORD [rbp - 24], 1	; s++
.s_loop:				;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		; deref s
	test    al, al			; s == NULL?
	jne     .cast_reject_to_temp	;
.return_i:				;
	mov     rax, QWORD [rbp - 8]	;
	pop     rbp			; epilogue
	ret				;
