AS = nasm
CC = gcc
LD = ld

CFLAGS = -g -ffreestanding -mno-red-zone -m64 -Iinclude
LDFLAGS = -m elf_x86_64 -T linker.ld
ASFLAGS = -f elf64 -g -F dwarf

all: kernel.elf

boot.o: boot/boot.asm
	$(AS) $(ASFLAGS) boot/boot.asm -o boot.o

kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel.o

terminal.o: kernel/terminal.c
	$(CC) $(CFLAGS) -c kernel/terminal.c -o terminal.o

idt.o: kernel/idt.c
	$(CC) $(CFLAGS) -c kernel/idt.c -o idt.o

idt_load.o: kernel/idt_load.asm
	$(AS) $(ASFLAGS) kernel/idt_load.asm -o idt_load.o

isr.o: kernel/isr.asm
	$(AS) $(ASFLAGS) kernel/isr.asm -o isr.o

read_cr2.o: kernel/read_cr2.asm
	$(AS) $(ASFLAGS) kernel/read_cr2.asm -o read_cr2.o

io.o: kernel/io.asm
	$(AS) $(ASFLAGS) kernel/io.asm -o io.o

pic.o: kernel/pic.c
	$(CC) $(CFLAGS) -c kernel/pic.c -o pic.o

pit.o: kernel/pit.c
	$(CC) $(CFLAGS) -c kernel/pit.c -o pit.o

kernel.elf: boot.o kernel.o terminal.o idt.o idt_load.o isr.o read_cr2.o io.o pic.o pit.o linker.ld
	$(LD) $(LDFLAGS) -o kernel.elf boot.o terminal.o idt.o idt_load.o isr.o read_cr2.o kernel.o io.o pic.o pit.o

iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o anios.iso iso

run: iso
	DISPLAY=:1 qemu-system-x86_64 -cdrom anios.iso -display gtk

debug: iso
	# DISPLAY=:1 qemu-system-x86_64 -cdrom anios.iso -display gtk -S -s
	DISPLAY=:1 qemu-system-x86_64 -cdrom anios.iso -display gtk -S -s -no-reboot

clean:
	rm -f *.o kernel.elf anios.iso
