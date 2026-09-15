set auto-load local-gdbinit on
add-auto-load-safe-path .

# Set architecture if cross-compiling (e.g., x86_64 or i386)
set architecture i386:x86-64

# Connect to QEMU's GDB stub
target remote :1234

# Load kernel symbol table
file kernel.elf

