section .multiboot_header

align 8; multiboot specification says header needs to be 8 byte aligned

header_start:
	dd 0xE85250D6; multiboot magic number
	dd 0; multiboot architecture field: i386
	dd header_end - header_start; comes out to 0x00000010 (header length)
	dd -(0xE85250D6 + 0 + (header_end - header_start)); checksum

	;  Multiboot2 end tag
	dw 0
	dw 0
	dd 8

header_end:

	section .text

	bits 32

	;      _start is kernel entry point.
	;      GRUB -> aniOS loaded -> _start
	global _start

	extern kernel_main

_start:
	; disable maskable interrupts using clear interrupt flag (cli)
	; since the kernels still getting setup, it would be pretty dumb if we have a keyboard interrupt suddenly mess it up.
	cli

	lgdt [gdt_descriptor]

	xor eax, eax

	;   pml4 -> pdpt -> pd
	mov eax, pdpt
	or  eax, 0x3
	mov [pml4], eax
	mov dword [pml4 + 4], 0

	mov eax, pd
	or  eax, 0x3
	mov [pdpt], eax
	mov dword [pdpt + 4], 0

	mov edi, pd
	xor eax, eax; will act as current physical address
	mov ecx, 512; this will act as the loop counter

.map_pd:
	mov edx, eax
	;   FLAGS
	;   0x01 → present
	;   0x02 → writable
	;   0x80 → page size (2 MiB)
	;   0x01 + 0x02 + 0x80 = 0x83
	or  edx, 0x83
	mov [edi], edx
	mov dword [edi + 4], 0

	add eax, 0x200000; advance physical address by 2 MiB
	add edi, 8; similarly add 8 bytes for one pd entry

	loop .map_pd

	mov eax, pml4
	mov cr3, eax; cr3 register points to top level page table

	;   enabling Physical Address Extension
	mov eax, cr4; cr4 contains bunch of config bits for the cpu
	or  eax, 1 << 5; set bit 5 (PAE bit)
	mov cr4, eax

	;   enabling long mode
	mov ecx, 0xC0000080; msr address for EFER
	rdmsr
	or  eax, 1 << 8; set lme bit
	wrmsr

	;   enabling paging
	mov eax, cr0
	or  eax, 1 << 31; set paging enable bit
	mov cr0, eax

	jmp 0x08: long_mode_start; use gdt entry 1 as code segment (0x08)

bits 64

long_mode_start:
	mov  rsp, stack_top
	call kernel_main

.hang:
	hlt; cpu sleeps until there is an interrupt or event
	jmp  .hang

section .rodata

align 8

gdt_start:

null_descriptor:
	dq 0; 8 byte zero entry in gdt

code_descriptor:
	dq 0x00AF9A000000FFFF

data_descriptor:
	dq 0x00CF92000000FFFF

gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start - 1; size of gdt
	dq gdt_start; address of gdt

	section .bss

	align 4096

pml4:
	resb 4096

	align 4096

pdpt:
	resb 4096

	align 4096

pd:
	resb 4096

	align 16; stack alignment is specified at divisible by 16

stack_bottom:
	resb 16384; reserve 16kb of space for our stack

stack_top:
