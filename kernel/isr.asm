	; flow of interrrupt:
	; int n -> cpu -> idt[n] -> ISRn -> interrupt_handler() [c] -> iretq -> back where we were

	bits 64

	section .text

	global isr0
	extern interrupt_handler

	; we push all gprs into rsp to preserve them before the interrupt call
	; C can then use the registers however they want while handling the interrupt
	; once its done with the interrupt, we pop all of them to return back to where we were
	; ORDER OF REGISTERS IS IMPORTANT

isr0:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	;    8 byte dummy push for stack alignment to 16 bytes
	push qword 0

	add  rsp, 8; undo dummy push
	mov  rdi, rsp
	call interrupt_handler

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax

	iretq
