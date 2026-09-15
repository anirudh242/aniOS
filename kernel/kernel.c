#include "idt.h"
#include "io.h"
#include "pic.h"
#include "pit.h"
#include "terminal.h"
#include "timer.h"
#include <stdint.h>

void kernel_main(void) {
    terminal_initialize();

    terminal_write("aniOS\n");
    terminal_write("64-bit kernel initialized\n");

    idt_initialize();
    pic_remap();
    pit_init(100);
    asm volatile("sti");

    terminal_write("Before sleep\n");

    timer_sleep(200);

    terminal_write("After sleep\n");

    // asm volatile("int $0");
    // volatile int a = 10;
    // volatile int b = 0;
    // volatile int c = a / b;
    // (void)c;

    // terminal_write("Before invalid opcode\n");
    // asm volatile("ud2");
    // terminal_write("After invalid opcode\n");

    // terminal_write("Before general protection fault\n");
    //
    // asm volatile("mov $0x18, %%ax\n"
    //              "mov %%ax, %%ds\n"
    //              :
    //              :
    //              : "rax");
    //
    // terminal_write("After general protection fault\n");

    // terminal_write("Before page fault\n");
    // volatile uint64_t *bad_address = (uint64_t *)0x0000400000000000ULL;
    // uint64_t value = *bad_address;
    // (void)value;

    // outb(0x80, 0);
    // uint8_t value = inb(0x21);
}
