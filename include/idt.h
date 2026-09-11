#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define ISR_LIST                                                               \
    X(0)                                                                       \
    X(1)                                                                       \
    X(2)                                                                       \
    X(3)                                                                       \
    X(4)                                                                       \
    X(5)                                                                       \
    X(6)                                                                       \
    X(7)                                                                       \
    X(8)                                                                       \
    X(9)                                                                       \
    X(10)                                                                      \
    X(11)                                                                      \
    X(12)                                                                      \
    X(13)                                                                      \
    X(14)                                                                      \
    X(15)                                                                      \
    X(16)                                                                      \
    X(17)                                                                      \
    X(18)                                                                      \
    X(19)                                                                      \
    X(20)                                                                      \
    X(21)                                                                      \
    X(22)                                                                      \
    X(23)                                                                      \
    X(24)                                                                      \
    X(25)                                                                      \
    X(26)                                                                      \
    X(27)                                                                      \
    X(28)                                                                      \
    X(29)                                                                      \
    X(30)                                                                      \
    X(31)

#define X(n) extern void isr##n(void);
ISR_LIST
#undef X

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

struct interrupt_frame {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t vector;
    uint64_t error_code;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
};

void idt_initialize(void);
void idt_load(uint64_t address);

#endif
