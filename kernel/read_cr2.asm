bits 64

section .text

global read_cr2

read_cr2:
	mov rax, cr2
	ret
