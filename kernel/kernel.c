#include "idt.h"
#include "terminal.h"
void kernel_main(void) {
    terminal_initialize();

    terminal_write("aniOS\n");
    terminal_write("64-bit kernel initialized\n");

    idt_initialize();

    asm volatile("int $0");

    terminal_write("Back from interrupt\n");
}
