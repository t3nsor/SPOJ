; 2009-01-01
%macro getc 1
	mov	%1,byte [esi]
	inc	esi
%endmacro
%macro putc 1
	mov	byte [edi],%1
	inc 	edi
%endmacro

section	.text
    global _start			;must be declared for linker (ld)

_start:
	mov	eax,3			;sys_read
	mov	ebx,0			;stdin
	mov	ecx,indata		;buffer
	mov	edx,10000000
	mov	esi,indata		;input pointer
	int	0x80
	mov	edi,outdata		;output pointer
_up:
	call	getint
	cmp	eax,42
	jz	_down
	call	putint
	putc	0x0A
	jmp	_up
_down:
	mov	eax,4			;sys_write
	mov	ebx,1			;stdout
	mov	ecx,outdata		;buffer
	mov	edx,edi			;size of output
	sub	edx,outdata
	int	0x80
	mov	eax,1			;sys_exit
	mov	ebx,0			;exit code 0
	int	0x80

getint:					;returns eax, destroys ebx and ecx
	mov	eax,0			;initial value
	mov	ecx,0
	mov	bl,0			;not negative
	_getint_loop:
		getc	cl
		cmp	cl,'-'
		jnz	_getint_nonn
		mov	bl,1			;negative
		jmp	_getint_loop
		_getint_nonn:
		cmp	cl,32			;whitespace?
		jle	_getint_out		;yes
		lea	eax,[eax+eax*4]		;multiply by 10d
		add	eax,eax
		add	eax,ecx
		sub	eax,'0'
		jmp	_getint_loop		;back to top
	_getint_out:
	cmp	bl,0
	jz	_getint_nonn2
	neg	eax
	_getint_nonn2:
	ret

putint:					;destroys ebx,ecx,edx
	cmp	eax,0
	jge	_putint_nonn
	neg	eax
	putc	'-'
	_putint_nonn:
	mov	ebx,eax
	cmp	eax,10
	jl	_putint_singledigit
	mov	edx,0
	push	eax
	mov	ecx,10
	div	ecx
	push	eax
	call	putint
	pop	eax
	lea	eax,[eax+eax*4]
	add	eax,eax
	pop	ebx
	sub	ebx,eax
	_putint_singledigit:
	add	bl,'0'
	putc	bl
	ret
section	.bss
indata	resb	10000000
outdata	resb	10000000

