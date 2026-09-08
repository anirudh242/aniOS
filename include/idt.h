#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_entry {
    uint16_t offset_1; // offset bits 0..15
    uint16_t selector; // a code segment selector in GDT or LDT
    uint8_t
        ist; // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
    uint8_t type_attributes; // gate type, dpl, and p fields
    uint16_t offset_2;       // offset bits 16..31
    uint32_t offset_3;       // offset bits 32..63
    uint32_t zero;           // reserved
} __attribute__((packed)); // packed removes extra padding c applies on default

struct idt_descriptor {
    uint16_t limit; // size of idt
    uint64_t base;  // starting memory address
} __attribute__((packed));

void idt_initialize(void);
void idt_load(uint64_t address);

extern void isr0(void);

#endif
