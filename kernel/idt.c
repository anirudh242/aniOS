#include "idt.h"
#include "terminal.h"
#include <stddef.h>
#include <stdint.h>

/* note: static vars get stored in .bss (zero initialized) and .data
 * (initialized) */
static struct idt_entry idt[256];

static struct idt_descriptor idt_descriptor;

static void idt_set_gate(int vector, uint64_t handler) {
    idt[vector].offset_1 = handler & 0xFFFF; // bottom 16 bits
    idt[vector].selector = 0x08;             // gdt code segment
    idt[vector].ist = 0;                     // no ist for this
    idt[vector].type_attributes = 0x8E; // flags set for present interrupt gate
    idt[vector].offset_2 = (handler >> 16) & 0xFFFF;     // next 16 bits
    idt[vector].offset_3 = (handler >> 32) & 0xFFFFFFFF; // last 32 bits
    idt[vector].zero = 0;
}

void idt_initialize(void) {
    for (size_t i = 0; i < 256; i++) {
        idt[i].offset_1 = 0;
        idt[i].selector = 0;
        idt[i].ist = 0;
        idt[i].offset_2 = 0;
        idt[i].zero = 0;
        idt[i].offset_3 = 0;
    }

    idt_set_gate(0, (uint64_t)isr0);

    idt_descriptor.limit = sizeof(idt) - 1;
    idt_descriptor.base = (uint64_t)idt;

    idt_load((uint64_t)&idt_descriptor);
}

void interrupt_handler(struct interrupt_frame *frame) {
    if (frame->vector == 0) {
        terminal_write("EXCEPTION: Divide Error\n");

    } else {
        terminal_write("EXCEPTION: Unknown\n");
    }

    terminal_write("Vector: ");
    terminal_write_hex(frame->vector);
    terminal_putchar('\n');

    terminal_write("RIP: ");
    terminal_write_hex(frame->rip);
    terminal_putchar('\n');

    terminal_write("SYSTEM HALTED\n");

    for (;;) {
        asm volatile("hlt");
    }
}
