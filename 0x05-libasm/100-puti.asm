BITS 64

extern asm_strlen		; import `asm_strlen` function
extern asm_putc			; import `asm_putc` function
global asm_puti			; export `asm_puti` function
section .text

	;; /**
	;;  * asm_puti - prints a signed integer on the standard output
	;;  *
	;;  * @n: integer to be printed
	;;  * Return: total number of bytes written on the standard output
	;;  */
	;; size_t asm_puti(int n)
	;; {
	;; 	unsigned int pow_10 = 1, temp;
	;;	size_t i = 0, chr_cnt = 1;
	;;
	;; 	if (n < 0)
	;; 	{
	;; 		asm_putc('-');
	;; 		i++;
	;; 		chr_cnt++;
	;; 		temp = (unsigned int)-n;
	;; 	}
	;; 	else
	;; 		temp = (unsigned int)n;
	;;
	;; 	while (pow_10 <= (temp / 10))
	;; 	{
	;; 		pow_10 *= 10;
	;; 		chr_cnt++;
	;; 	}
	;;
	;; 	for (; i < chr_cnt; i++)
	;; 	{
	;; 		asm_putc(temp / pow_10 + '0');
	;; 		temp %= pow_10;
	;; 		pow_10 /= 10;
	;; 	}
	;;
	;; 	return (chr_cnt);
	;; }

asm_puti:
	push    rbp			; prologue
	mov     rbp, rsp		;
	sub     rsp, 48			; reserve 48 bytes on stack
	mov     DWORD [rbp - 36], edi	; reserve space on stack for n
	mov     DWORD [rbp - 4], 1	; reserve space on stack for pow_10 = 1
	mov     QWORD [rbp - 16], 0	; reserve space on stack for i = 0
	mov     QWORD [rbp - 24], 1	; reserve space on stack for chr_cnt = 1
	cmp     DWORD [rbp - 36], 0	; n < 0?
	jns     .cast_n_to_temp		;
	mov     edi, 45			; '-'
	call    asm_putc		;
	add     QWORD [rbp - 16], 1	; i++
	add     QWORD [rbp - 24], 1	; chr_cnt++
	mov     eax, DWORD [rbp - 36]	;
	neg     eax			; negates n with 2's complement
	mov     DWORD [rbp - 8], eax	; reserve space on stack for temp = -n
	jmp     .pow_10_loop_test	;
.cast_n_to_temp:			;
	mov     eax, DWORD [rbp - 36]	;
	mov     DWORD [rbp - 8], eax	; reserve space on stack for temp = n
	jmp     .pow_10_loop_test	;
.pow_10_loop:				;
	mov     eax, DWORD [rbp - 4]	;
	imul    eax, 10			; pow_10 * 10
	mov     DWORD [rbp - 4], eax	; result is new pow_10
	add     QWORD [rbp - 24], 1	; chr_cnt++
.pow_10_loop_test:			;
	mov     eax, DWORD [rbp - 8]	;
	mov     edx, 0			; init div return register edx
	mov	rcx, 10			; div needs register operand
	div     rcx			; temp / 10
	cmp     eax, DWORD [rbp - 4]	; >= pow_10
	jnb     .pow_10_loop		;
	jmp     .print_loop_test	;
.print_loop:				;
	mov     eax, DWORD [rbp - 8]	;
	mov     edx, 0			; init div return register edx
	div     DWORD [rbp - 4]		; temp / pow_10
	add     eax, 48			; + '0'
	mov     edi, eax		; result into edi for call param
	call    asm_putc		;
	mov     eax, DWORD [rbp - 8]	;
	mov     edx, 0			; init div return register edx
	div     DWORD [rbp - 4]		; temp % pow_10
	mov     DWORD [rbp - 8], edx	; remainder (edx) is new temp
	mov     eax, DWORD [rbp - 4]	;
	mov     edx, 0			; init div return register edx
	mov	rcx, 10			; div needs register operand
	div 	rcx			; pow_10 / 10
	mov     DWORD [rbp - 4], eax	; quotient (rax) is new pow_10
	add     QWORD [rbp - 16], 1	; i++
.print_loop_test:			;
	mov     rax, QWORD [rbp - 16]	;
	cmp     rax, QWORD [rbp - 24]	;
	jb      .print_loop		;
	mov     rax, QWORD [rbp - 24]	;
	leave				;
	ret				;
