AS = nasm
CC = gcc
LD = ld

CFLAGS = -ffreestanding -mno-red-zone -m64
LDFLAGS = -m elf_x86_64 -T linker.ld

all: kernel.elf

boot.o: boot/boot.asm
	$(AS) -f elf64 boot/boot.asm -o boot.o

kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel.o

kernel.elf: boot.o kernel.o linker.ld
	$(LD) $(LDFLAGS) -o kernel.elf boot.o kernel.o

iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o anios.iso iso

run: iso
	DISPLAY=:1 qemu-system-x86_64 -cdrom anios.iso -display gtk

clean:
	rm -f boot.o kernel.o kernel.elf anios.iso
