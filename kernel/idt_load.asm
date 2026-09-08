bits 64

section .text

global idt_load

idt_load:
	lidt [rdi]; rdi -> first arg of function call
	ret
