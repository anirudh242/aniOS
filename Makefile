AS      := nasm
CC      := gcc
LD      := ld
GRUB    := grub-mkrescue
QEMU    := qemu-system-x86_64

# Directories
BUILD   := build
ISO_DIR := iso

# Output files
KERNEL  := $(BUILD)/kernel.elf
ISO     := anios.iso

# Source files
C_SRCS   := $(wildcard kernel/*.c)
ASM_SRCS := $(wildcard boot/*.asm kernel/*.asm)

C_OBJS   := $(patsubst %.c,$(BUILD)/%.o,$(C_SRCS))
ASM_OBJS := $(patsubst %.asm,$(BUILD)/%.o,$(ASM_SRCS))

OBJS := $(ASM_OBJS) $(C_OBJS)
DEPS := $(OBJS:.o=.d)

# Compiler flags
CFLAGS := \
	-g \
	-ffreestanding \
	-mno-red-zone \
	-m64 \
	-Iinclude \
	-Wall \
	-Wextra \
	-Wpedantic \
	-MMD \
	-MP

# Assembler flags
ASFLAGS := \
	-f elf64 \
	-g \
	-F dwarf

# Linker flags
LDFLAGS := \
	-m elf_x86_64 \
	-T linker.ld

# QEMU flags
QEMU_FLAGS := \
	-cdrom $(ISO) \
	-display gtk \
	-no-reboot

# Default target
.PHONY: all
all: $(ISO)

# Compile C files
$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "  CC      $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble ASM files
$(BUILD)/%.o: %.asm
	@mkdir -p $(dir $@)
	@echo "  AS      $<"
	$(AS) $(ASFLAGS) $< -o $@

# Link the kernel
$(KERNEL): $(OBJS) linker.ld
	@mkdir -p $(dir $@)
	@echo "  LD      $@"
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# Create the ISO
$(ISO): $(KERNEL)
	@echo "  ISO     $@"
	cp $(KERNEL) $(ISO_DIR)/boot/kernel.elf
	$(GRUB) -o $@ $(ISO_DIR)

# Run aniOS
.PHONY: run
run: $(ISO)
	DISPLAY=:1 $(QEMU) $(QEMU_FLAGS)

# Run aniOS with GDB
.PHONY: debug
debug: $(ISO)
	DISPLAY=:1 $(QEMU) $(QEMU_FLAGS) -S -s

# Remove build files
.PHONY: clean
clean:
	@echo "  CLEAN"
	rm -rf $(BUILD) $(ISO)

# Include generated dependencies
-include $(DEPS)
