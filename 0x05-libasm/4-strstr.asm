BITS 64

global asm_strstr			; EXPORT `asm_strstr` function
section .text

	;; char *asm_strstr(const char *haystack, const char *needle)
	;; {
	;; 	char *hs = (char *)haystack, *ndl = (char *)needle,
	;; 		*temp = NULL;
	;;
	;; 	if (!*hs)
	;; 		return (NULL);
	;;
	;;	for (; *hs; hs++)
	;; 	{
	;;		for (temp = ndl; *temp; temp++)
	;; 		{
	;; 			if (*temp != *(hs + (temp - ndl)))
	;;				break;
	;; 		}
	;;
	;; 		if (!*temp)
	;;              	return (hs);
	;; 	}
	;;
	;;	return (NULL);
	;; }

asm_strstr:
	push    rbp			; prologue
	mov     rbp, rsp		;
	mov     QWORD [rbp - 40], rdi	; reserve space on stack for haystack
	mov     QWORD [rbp - 48], rsi	; reserve space on stack for needle
	mov     rax, QWORD [rbp - 40]	; cast haystack
	mov     QWORD [rbp - 8], rax	; reserve space on stack for hs
	mov     rax, QWORD [rbp - 48]	; cast needle
	mov     QWORD [rbp - 24], rax	; reserve space on stack for ndl
	mov     QWORD [rbp - 16], 0	; reserve space on stack for temp == NULL
        mov     rax, QWORD [rbp - 8] 	;
	movzx   eax, BYTE [rax]	     	;
	test    al, al		     	; *hs == '\0'?
	jne     .hs_loop_test		;
        jmp     .return_null		;
.temp_loop_setup:			;
	mov     rax, QWORD [rbp - 24]	;
	mov     QWORD [rbp - 16], rax	; temp = ndl
	jmp     .temp_loop_test		;
.temp_loop:				;
	mov     rax, QWORD [rbp - 16]	;
	movzx   edx, BYTE [rax]		; deref temp to edx
	mov     rcx, QWORD [rbp - 16]	;
	mov     rax, QWORD [rbp - 24]	;
	sub     rcx, rax		; temp - ndl
	mov     rax, rcx		;
	mov     rcx, rax		; store temp - ndl in rcx
	mov     rax, QWORD [rbp - 8]	;
	add     rax, rcx		; hs + (temp - ndl)
	movzx   eax, BYTE [rax]		; deref result
	cmp     dl, al			; result == *temp?
	je      .inc_temp		;
	jmp     .temp_test_after_loop	;
.inc_temp:				;
	add     QWORD [rbp - 16], 1	; temp++
.temp_loop_test:			;
	mov     rax, QWORD [rbp - 16]	;
	movzx   eax, BYTE [rax]		;
	test    al, al			; *temp == '\0'?
	jne     .temp_loop		;
.temp_test_after_loop:			;
	mov     rax, QWORD [rbp - 16]	;
	movzx   eax, BYTE [rax]		;
	test    al, al			; *temp == '\0'?
	jne     .inc_hs			;
	mov     rax, QWORD [rbp - 8]	; return hs
	jmp     .return			;
.inc_hs:				;
	add     QWORD [rbp - 8], 1	;
.hs_loop_test:				;
	mov     rax, QWORD [rbp - 8]	;
	movzx   eax, BYTE [rax]		;
	test    al, al			; *hs == '\0'?
	jne     .temp_loop_setup	;
.return_null:				;
	mov     eax, 0			; return NULL
.return:				;
	pop     rbp			; epilogue
	ret				;
