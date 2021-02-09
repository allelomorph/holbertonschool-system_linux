BITS 64

global asm_putc			; export `asm_putc` function
section .text

	;; size_t asm_putc(int c)
	;; {
	;;         return (write(1, &c, 1));
	;; }

asm_putc:
	push    rbp			; prologue
	mov     rbp, rsp		;
	sub     rsp, 16			; reserve 16 bytes on stack
	mov     DWORD [rbp - 4], edi	; store destination index in c
	lea     rax, [rbp - 4]		; store address of c in rax
	mov     edx, 1			; write param 2: 1 byte
	mov     rsi, rax		; write param 1: &c into source index
	mov     edi, 1			; write param 0: 1 for stdout fd
	mov	eax, 1			; write syscall code == 1
	syscall				; call write
	leave				; return from write call
	ret
