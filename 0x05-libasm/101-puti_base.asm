BITS 64

extern asm_strlen		; import `asm_strlen` function
extern asm_putc			; import `asm_putc` function
global asm_puti_base		; export `asm_puti_base` function
section .text


asm_puti_base:
	push    rbp			;
	mov     rbp, rsp		;
	sub     rsp, 48			;
	mov     DWORD [rbp - 36], edi	;
	mov     QWORD [rbp - 48], rsi	;
	mov     DWORD [rbp - 4], 1	;
	mov     QWORD [rbp - 16], 0	;
	mov     QWORD [rbp - 24], 1	;
	mov     rax, QWORD [rbp - 48]	;
	mov     rdi, rax		;
	call    asm_strlen		;
	mov     DWORD [rbp - 28], eax	;
	cmp     DWORD [rbp - 28], 1	; bs < 2?
	ja      .n_neg_test		;
	mov     eax, 0			;
	jmp     .return			;
.n_neg_test:				;
	cmp     DWORD [rbp - 36], 0	; n < 0?
	jns     .n_pos			;
	mov     edi, 45			;
	call    asm_putc		;
	add     QWORD [rbp - 16], 1	;
	add     QWORD [rbp - 24], 1	;
	mov     eax, DWORD [rbp - 36]	;
	neg     eax			;
	mov     DWORD [rbp - 8], eax	;
	jmp     .pow_of_bs_loop_test	;
.n_pos:					;
	mov     eax, DWORD [rbp - 36]	;
	mov     DWORD [rbp - 8], eax	;
	jmp     .pow_of_bs_loop_test	;
.pow_of_bs_loop:			;
	mov     eax, DWORD [rbp - 4]	;
	imul    eax, DWORD [rbp - 28]	;
	mov     DWORD [rbp - 4], eax	;
	add     QWORD [rbp - 24], 1	; chr_cnt++
.pow_of_bs_loop_test:			;
	mov     eax, DWORD [rbp - 8]	;
	mov     edx, 0			;
	div     DWORD [rbp - 28]	;
	cmp     eax, DWORD [rbp - 4]	;
	jnb     .pow_of_bs_loop		;
	jmp     .print_loop_test	;
.print_loop:				;
	mov     eax, DWORD [rbp - 8] 	;
	mov     edx, 0		     	;
	div     DWORD [rbp - 4]	    	;
	mov     edx, eax		;
	mov     rax, QWORD [rbp - 48]	;
	add     rax, rdx		;
	movzx   eax, BYTE [rax]		;
	movsx   eax, al			;
	mov     edi, eax		;
	call    asm_putc		;
	mov     eax, DWORD [rbp - 8]	;
	mov     edx, 0			;
	div     DWORD [rbp - 4]		;
	mov     DWORD [rbp - 8], edx	;
	mov     eax, DWORD [rbp - 4]	;
	mov     edx, 0			;
	div     DWORD [rbp - 28]	;
	mov     DWORD [rbp - 4], eax	;
	add     QWORD [rbp - 16], 1	;
.print_loop_test:			;
	mov     rax, QWORD [rbp - 16]	;
	cmp     rax, QWORD [rbp - 24]	;
	jb      .print_loop		;
	mov     rax, QWORD [rbp - 24]	;
.return:				;
	leave				;
	ret				;
