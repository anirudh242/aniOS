	; flow of interrrupt:
	; int n -> cpu -> idt[n] -> ISRn -> interrupt_handler() [c] -> iretq -> back where we were

	bits 64

	section .text
	%macro  ISR_NO_ERROR 1
	global  isr%1

isr%1:
	push qword 0
	push qword %1
	jmp  isr_common
%endmacro

%macro ISR_ERROR 1
global isr%1

isr%1:
	push qword %1
	jmp  isr_common
%endmacro

extern interrupt_handler

	ISR_NO_ERROR 0
	ISR_NO_ERROR 1
	ISR_NO_ERROR 2
	ISR_NO_ERROR 3
	ISR_NO_ERROR 4
	ISR_NO_ERROR 5
	ISR_NO_ERROR 6
	ISR_NO_ERROR 7

	ISR_ERROR 8

	ISR_NO_ERROR 9

	ISR_ERROR 10
	ISR_ERROR 11
	ISR_ERROR 12
	ISR_ERROR 13
	ISR_ERROR 14

	ISR_NO_ERROR 15
	ISR_NO_ERROR 16

	ISR_ERROR 17

	ISR_NO_ERROR 18
	ISR_NO_ERROR 19
	ISR_NO_ERROR 20

	ISR_ERROR 21

	ISR_NO_ERROR 22
	ISR_NO_ERROR 23
	ISR_NO_ERROR 24
	ISR_NO_ERROR 25
	ISR_NO_ERROR 26
	ISR_NO_ERROR 27
	ISR_NO_ERROR 28

	ISR_ERROR 29
	ISR_ERROR 30

	ISR_NO_ERROR 31
	ISR_NO_ERROR 32

isr_common:
	;    we push all gprs into rsp to preserve them before the interrupt call
	;    C can then use the registers however they want while handling the interrupt
	;    once its done with the interrupt, we pop all of them to return back to where we were
	;    ORDER OF REGISTERS IS IMPORTANT
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
	;    check if rsp is 16-bit aligned
	;    if bit 3 is set, rsp is 8 mod 16
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

	add rsp, 16; removes vector and error code
	iretq
