#include "idt.h"
#include "terminal.h"
#include <stdint.h>
void kernel_main(void) {
    terminal_initialize();

    terminal_write("aniOS\n");
    terminal_write("64-bit kernel initialized\n");

    idt_initialize();

    // asm volatile("int $0");
    // volatile int a = 10;
    // volatile int b = 0;
    // volatile int c = a / b;
    // (void)c;

    // terminal_write("Before invalid opcode\n");
    // asm volatile("ud2");
    // terminal_write("After invalid opcode\n");

    terminal_write("Before general protection fault\n");

    asm volatile("mov $0x18, %%ax\n"
                 "mov %%ax, %%ds\n"
                 :
                 :
                 : "rax");

    terminal_write("After general protection fault\n");
}
