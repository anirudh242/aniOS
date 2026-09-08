AS = nasm
CC = gcc
LD = ld

CFLAGS = -ffreestanding -mno-red-zone -m64 -Iinclude
LDFLAGS = -m elf_x86_64 -T linker.ld

all: kernel.elf

boot.o: boot/boot.asm
	$(AS) -f elf64 boot/boot.asm -o boot.o

kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel.o

terminal.o: kernel/terminal.c
	$(CC) $(CFLAGS) -c kernel/terminal.c -o terminal.o

idt.o: kernel/idt.c
	$(CC) $(CFLAGS) -c kernel/idt.c -o idt.o

idt_load.o: kernel/idt_load.asm
	$(AS) -f elf64 kernel/idt_load.asm -o idt_load.o

isr.o: kernel/isr.asm
	$(AS) -f elf64 kernel/isr.asm -o isr.o

kernel.elf: boot.o kernel.o terminal.o idt.o idt_load.o isr.o linker.ld
	$(LD) $(LDFLAGS) -o kernel.elf boot.o terminal.o idt.o idt_load.o isr.o kernel.o

iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o anios.iso iso

run: iso
	DISPLAY=:1 qemu-system-x86_64 -cdrom anios.iso -display gtk

clean:
	rm -f *.o kernel.elf anios.iso
