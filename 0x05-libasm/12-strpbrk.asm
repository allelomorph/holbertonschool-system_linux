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
	;; 	/* !temp: accept == NULL or !s, !(*temp): accept == "" */
	;; 	if (temp && *temp)
	;; 		return ((char *)s);
	;; 	else
	;; 		return (NULL);
	;; }

asm_strpbrk:
	push    rbp				; prologue
	mov     rbp, rsp			;
	mov     QWORD [rbp - 24], rdi		; reserve stack memory for s
	mov     QWORD [rbp - 32], rsi		; reserve stack memory for accept
	mov     QWORD [rbp - 8], 0		; reserve stack memory for temp = NULL
	jmp     .s_loop_test			;
.s_loop:					;
	mov     rax, QWORD [rbp - 32]		;
	mov     QWORD [rbp - 8], rax		;
	jmp     .temp_loop			;
.inc_temp:					;
	add     QWORD [rbp - 8], 1		;
.temp_loop:					;
	mov     rax, QWORD [rbp - 8]		;
	movzx   eax, BYTE [rax]			;
	test    al, al				;
	je      .test_temp_after_temp_loop	;
	mov     rax, QWORD [rbp - 8]		;
	movzx   edx, BYTE [rax]			;
	mov     rax, QWORD [rbp - 24]		;
	movzx   eax, BYTE [rax]			;
	cmp     dl, al				;
	jne     .inc_temp			;
.test_temp_after_temp_loop:			;
	mov     rax, QWORD [rbp - 8]		;
	movzx   eax, BYTE [rax]			;
	test    al, al				;
	je      .inc_s				;
	jmp     .test_temp_after_s_loop		;
.inc_s:						;
	add     QWORD [rbp - 24], 1		;
.s_loop_test:					;
	mov     rax, QWORD [rbp - 24]		;
	movzx   eax, BYTE [rax]			;
	test    al, al				;
	jne     .s_loop				;
.test_temp_after_s_loop:			;
        cmp     QWORD [rbp - 8], 0 		;
	je      .return_null	   		;
	mov     rax, QWORD [rbp - 8] 		;
	movzx   eax, BYTE [rax]	     		;
	test    al, al		     		;
	je      .return_null			;
	mov     rax, QWORD [rbp - 24] 		;
	jmp     .return		      		;
.return_null:			      		;
	mov     eax, 0			      	;
.return:					;
	pop     rbp				; epilogue
	ret					;
