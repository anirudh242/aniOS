	; flow of interrrupt:
	; int n -> cpu -> idt[n] -> ISRn -> interrupt_handler() [c] -> iretq -> back where we were

	bits 64

	section .text

	global isr0
	extern interrupt_handler

isr0:
	call interrupt_handler
	iretq
