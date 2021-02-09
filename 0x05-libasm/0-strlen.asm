BITS 64

global asm_strlen			; EXPORT `asm_strlen` function
section .text

	;; size_t asm_strlen(const char *str)
	;; {
	;;         size_t len = 0;
	;;
	;;         for (; *str; str++)
	;;                 len++;
	;;
	;;         return (len);
	;; }

asm_strlen:
	push    rbp			; prologue: push stack base pointer to the stack (save a copy)
	mov     rbp, rsp		; prologue: make the current stack pointer the new base pointer
	mov     QWORD [rbp - 24], rdi	; reserve stack space for str
	mov     QWORD [rbp - 8], 0	; reserve stack space for len
	jmp     .test_str_null		;
.inc_str_len:				;
	add     QWORD [rbp - 8], 1	; len++
	add     QWORD [rbp - 24], 1	; str++
.test_str_null:				;
	mov     rax, QWORD [rbp - 24]	;
	movzx   eax, BYTE [rax]		; deref str
	test    al, al			; *str == '\0'?
	jne     .inc_str_len		;
	mov     rax, QWORD [rbp - 8]	; return len
	pop     rbp			; epilogue
	ret				;

	;; some assembly uses explicit hex values for jumps, but once linked those instruction addresses can change.
	;; better to use addresses relative to function label, or just labels for every jump
	;; https://stackoverflow.com/questions/775108/x86-jump-to-an-address
