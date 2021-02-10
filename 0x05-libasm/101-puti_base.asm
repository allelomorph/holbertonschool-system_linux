BITS 64

extern asm_strlen		; import `asm_strlen` function
extern asm_putc			; import `asm_putc` function
global asm_puti_base		; export `asm_puti_base` function
section .text

	;; size_t asm_puti_base(int n, const char *base)
	;; {
	;;	unsigned int pow_of_bs = 1, temp, bs;
	;;	size_t i = 0, chr_cnt = 1;
	;;
	;; 	bs = asm_strlen(base);
	;;
	;; 	if (bs < 2)
	;; 		return (0);
	;;
	;; 	if (n < 0)
	;; 	{
	;; 	        asm_putc('-');
	;; 		i++;
	;; 		chr_cnt++;
	;; 		temp = (unsigned int)-n;
	;; 	}
	;; 	else
	;; 		temp = (unsigned int)n;
	;;
	;; 	while (pow_of_bs <= (temp / bs))
	;; 	{
	;; 		pow_of_bs *= bs;
	;; 		chr_cnt++;
	;; 	}
	;;
	;; 	for (; i < chr_cnt; i++)
	;; 	{
	;; 	        asm_putc(base[temp / pow_of_bs]);
	;; 		temp %= pow_of_bs;
	;; 	        pow_of_bs /= bs;
	;; 	}
	;;
	;; 	return (chr_cnt);
	;; }

asm_puti_base:
	push    rbp			; prologue
	mov     rbp, rsp		;
	sub     rsp, 48			;
	mov     DWORD [rbp - 36], edi	; reserve space on stack for n
	mov     QWORD [rbp - 48], rsi	; reserve space on stack for base
	mov     DWORD [rbp - 4], 1	; reserve space on stack for pow_of_bs = 1
	mov     QWORD [rbp - 16], 0	; reserve space on stack for i = 0
	mov     QWORD [rbp - 24], 1	; reserve space on stack for chr_cnt = 1
	mov     rax, QWORD [rbp - 48]	;
	mov     rdi, rax		; base into rdi to act as param to asm_strlen
	call    asm_strlen		;
	mov     DWORD [rbp - 28], eax	; reserve space on stack for bs = asm_strlen(base)
	cmp     DWORD [rbp - 28], 1	; bs < 2?
	ja      .n_neg_test		;
	mov     eax, 0			; returning 0
	jmp     .return			;
.n_neg_test:				;
	cmp     DWORD [rbp - 36], 0	; n < 0?
	jns     .n_pos			;
	mov     edi, 45			; '-'
	call    asm_putc		;
	add     QWORD [rbp - 16], 1	; i++
	add     QWORD [rbp - 24], 1	; chr_cnt++
	mov     eax, DWORD [rbp - 36]	;
	neg     eax			; negates n with 2's complement
	mov     DWORD [rbp - 8], eax	; reserve space on stack for temp = -n
	jmp     .pow_of_bs_loop_test	;
.n_pos:					;
	mov     eax, DWORD [rbp - 36]	;
	mov     DWORD [rbp - 8], eax	; reserve space on stack for temp = n
	jmp     .pow_of_bs_loop_test	;
.pow_of_bs_loop:			;
	mov     eax, DWORD [rbp - 4]	;
	imul    eax, DWORD [rbp - 28]	; pow_of_bs * bs
	mov     DWORD [rbp - 4], eax	; result is new pow_of_bs
	add     QWORD [rbp - 24], 1	; chr_cnt++
.pow_of_bs_loop_test:			;
	mov     eax, DWORD [rbp - 8]	;
	mov     edx, 0			; init div return register edx
	div     DWORD [rbp - 28]	; temp / bs
	cmp     eax, DWORD [rbp - 4]	; >= pow_of_bs?
	jnb     .pow_of_bs_loop		;
	jmp     .print_loop_test	;
.print_loop:				;
	mov     eax, DWORD [rbp - 8] 	;
	mov     edx, 0		     	; init div return register edx
	div     DWORD [rbp - 4]	    	; temp / pow_of_bs
	mov     edx, eax		;
	mov     rax, QWORD [rbp - 48]	;
	add     rax, rdx		; base[div result] (base + div result)
	movzx   eax, BYTE [rax]		;
	movsx   eax, al			; deref (base + div result)
	mov     edi, eax		; that char to edi for call param
	call    asm_putc		;
	mov     eax, DWORD [rbp - 8]	;
	mov     edx, 0			; init div return register edx
	div     DWORD [rbp - 4]		; temp % pow_of_bs
	mov     DWORD [rbp - 8], edx	; remainder (edx) is new temp
	mov     eax, DWORD [rbp - 4]	;
	mov     edx, 0			; init div return register edx
	div     DWORD [rbp - 28]	; pow_of_bs / bs
	mov     DWORD [rbp - 4], eax	; quotient (eax) is new bs
	add     QWORD [rbp - 16], 1	; i++
.print_loop_test:			;
	mov     rax, QWORD [rbp - 16]	;
	cmp     rax, QWORD [rbp - 24]	; i < chr_cnt?
	jb      .print_loop		;
	mov     rax, QWORD [rbp - 24]	;
.return:				;
	leave				;
	ret				;
