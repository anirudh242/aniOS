#include "idt.h"
#include "io.h"
#include "terminal.h"
#include "timer.h"
#include <stddef.h>
#include <stdint.h>

/* note: static vars get stored in .bss (zero initialized) and .data
 * (initialized) */
static struct idt_entry idt[256];
static struct idt_descriptor idt_descriptor;

static const char *exception_names[33] = {
    [0] = "Divide Error",
    [1] = "Debug",
    [2] = "Non-Maskable Interrupt",
    [3] = "Breakpoint",
    [4] = "Overflow",
    [5] = "Bound Range Exceeded",
    [6] = "Invalid Opcode",
    [7] = "Device Not Available",
    [8] = "Double Fault",
    [9] = "Coprocessor Segment Overrun",
    [10] = "Invalid TSS",
    [11] = "Segment Not Present",
    [12] = "Stack-Segment Fault",
    [13] = "General Protection Fault",
    [14] = "Page Fault",
    [15] = "Reserved",
    [16] = "x87 Floating-Point Exception",
    [17] = "Alignment Check",
    [18] = "Machine Check",
    [19] = "SIMD Floating-Point Exception",
    [20] = "Virtualization Exception",
    [21] = "Control Protection Exception",
};

typedef void (*isr_handler_t)(void);

#define X(n) isr##n,
static isr_handler_t isr_handlers[34] = {ISR_LIST};
#undef X

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

    for (size_t i = 0; i < 34; i++) {
        idt_set_gate(i, (uint64_t)isr_handlers[i]);
    }

    idt_descriptor.limit = sizeof(idt) - 1;
    idt_descriptor.base = (uint64_t)idt;

    idt_load((uint64_t)&idt_descriptor);
}

// outb(0x20, 0x20) sends END OF INTERRUPT (EOI) signal to master PIC
void interrupt_handler(struct interrupt_frame *frame) {
    uint64_t fvec = frame->vector;

    // REMINDER: IRQ 32 and up are standard ISA IRQS starting where IRQ 32 is
    // mapped to 0, 33 to 1, and so on.

    // pit interrupt
    if (fvec == 32) {
        timer_tick();

        // if (timer_get_ticks() % 100 == 0) {
        // terminal_write("TICK\n");
        // }

        outb(0x20, 0x20);
        return;
    }

    // keyboard
    if (fvec == 33) {
        uint64_t scancode = inb(0x60); // keyboard controller data port

        terminal_write("KEYBOARD IRQ:");
        terminal_write_hex(scancode);
        terminal_write("\n");

        outb(0x20, 0x20);
        return;
    }

    if (fvec < 32 && exception_names[fvec]) {
        terminal_write("EXCEPTION: ");
        terminal_write(exception_names[fvec]);
        terminal_write("\n");
    } else {
        terminal_write("EXCEPTION: Unknown\n");
    }

    terminal_write("Vector: ");
    terminal_write_hex(fvec);
    terminal_putchar('\n');

    terminal_write("RIP: ");
    terminal_write_hex(frame->rip);
    terminal_putchar('\n');

    if (fvec == 14) {
        terminal_write("Faulting address: ");
        terminal_write_hex(read_cr2());
        terminal_putchar('\n');
    }

    terminal_write("SYSTEM HALTED\n");

    for (;;) {
        asm volatile("hlt");
    }
}
