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
	push qword 0; fake error code
	push qword 0; vector number
	jmp  isr_common

isr_common:
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

	mov rdi, rsp; save address of interrupt frame (arg 1 in c function)

	test rsp, 8; rsp AND 0x8
	;    this is set to 0 if its 16-bit aligned
	jz   .aligned

	;    otherwise, move down by 8 to align
	;    the call itself pushed an 8-byte return address
	;    afterwards add rsp, 8  restores the stack to before alignment adjustment
	sub  rsp, 8
	call interrupt_handler
	add  rsp, 8
	jmp  .restore

.aligned:
	call interrupt_handler

.restore:

	add rsp, 16; removes vector and error code

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
