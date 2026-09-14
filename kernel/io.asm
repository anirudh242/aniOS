bits 64

section .text

global outb
global inb

outb:
	;   out byte
	mov dx, di; low 16 bits of first arg -> dx
	mov al, sil; low 8 bytes of second arg -> al
	out dx, al; dx = port, al = byte to send
	ret

inb:
	;     in byte
	mov   dx, di; low 16 bits of first arg -> dx
	in    al, dx; dx = port, al = byte to send
	movzx eax, al
	ret
