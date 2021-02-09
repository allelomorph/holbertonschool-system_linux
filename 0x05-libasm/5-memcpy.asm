BITS 64

global asm_memcpy			; EXPORT `asm_memcpy` function
section .text

	;; void *asm_memcpy(void *dest, const void *src, size_t n)
	;; {
	;; 	size_t i;
	;; 	unsigned char *_dest = (unsigned char *)dest;
	;; 	unsigned char *_src = (unsigned char *)src;
	;;
	;; 	if (!_dest || !_src)
	;; 		return (dest);
	;;
	;;	for (i = 0; i < n; i++)
	;; 	{
	;; 		*(_dest + i) = *(_src + i);
	;; 	}
	;;
	;; 	return (dest);
	;; }

asm_memcpy:
	push    rbp			; prologue
	mov     rbp, rsp		;
	mov     QWORD [rbp - 40], rdi	; reserve space on stack for dest
	mov     QWORD [rbp - 48], rsi	; reserve space on stack for src
	mov     QWORD [rbp - 56], rdx	; reserve space on stack for n
	mov     rax, QWORD [rbp - 40]	; cast dest
	mov     QWORD [rbp - 16], rax	; reserve space on stack for _dest
	mov     rax, QWORD [rbp - 48]	; cast src
	mov     QWORD [rbp - 24], rax	; reserve space on stack for _src
	cmp     QWORD [rbp - 16], 0	; !_dest?
	je      .return_dest		;
	cmp     QWORD [rbp - 24], 0	; !_src?
	jne     .i_loop_setup		;
.return_dest:				;
	mov     rax, QWORD [rbp - 40]	;
	jmp     .return			;
.i_loop_setup:				;
	mov     QWORD [rbp - 8], 0	; reserve space on stack for i = 0
	jmp     .i_loop_test		;
.i_loop:				;
	mov     rax, QWORD [rbp - 8]	;
	mov     rdx, QWORD [rbp - 16]	;
	add     rdx, rax		; _dest + i
	mov     rax, QWORD [rbp - 8]	;
	mov     rcx, QWORD [rbp - 24]	;
	add     rax, rcx		; _src + i
	movzx   eax, BYTE [rax]		;
	mov     BYTE [rdx], al		;
	add     QWORD [rbp - 8], 1	; i++
.i_loop_test:				;
	mov     rax, QWORD [rbp - 8]	;
	cmp     rax, QWORD [rbp - 56]	; i < n
	jb      .i_loop			;
	mov     rax, QWORD [rbp - 40]	;
.return:				;
	pop     rbp			; epilogue
	ret				;
