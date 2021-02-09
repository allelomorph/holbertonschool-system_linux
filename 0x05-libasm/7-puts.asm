BITS 64

extern asm_strlen		; import `asm_strlen` function
global asm_puts			; export `asm_puts` function
section .text

	;; size_t asm_puts(const char *str)
	;; {
	;; 	size_t len;
	;;
	;; 	len = asm_strlen(str);
	;;
	;; 	return (write(1, str, len));
	;; }

asm_puts:
	push    rbp			; prologue
	mov     rbp, rsp		;
	sub     rsp, 32			; reserve 32 bytes on stack
	mov     QWORD [rbp - 24], rdi	; allocate char *str of 8 bytes at top of func stack
	mov     rax, QWORD [rbp - 24]	; begin: len = asm_strlen(str);
	mov     rdi, rax		;
	mov     eax, 0			;
	call    asm_strlen		;
	mov     DWORD [rbp - 4], eax	; end: len = asm_strlen(str);
	mov     edx, DWORD [rbp - 4]	; write param 2: return of asm_strlen
	mov     rax, QWORD [rbp - 24]	;
	mov     rsi, rax		; write param 1: str
	mov     edi, 1			; write param 0: 1 for stdout fd
	mov     eax, 1			; write syscall code: 1
	syscall				; call write (uses params in rsi, rdi, eax)
	cdqe				; write returns ssize_t, cast to size_t
	leave				; return from write call
	ret				;
