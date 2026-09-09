#include "idt.h"
#include "terminal.h"
void kernel_main(void) {
    terminal_initialize();

    terminal_write("aniOS\n");
    terminal_write("64-bit kernel initialized\n");

    idt_initialize();

    // asm volatile("int $0");
    volatile int a = 10;
    volatile int b = 0;
    volatile int c = a / b;
    (void)c;

    terminal_write("Back from interrupt\n");
}
