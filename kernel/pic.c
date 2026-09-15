#include "pic.h"
#include "io.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21

#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

// icw1 = initialisation
// icw2 = vector offset
// icw3 = relation b/w master slave
// icw4 = additional info (in this case, use x86 modern mode)
void pic_remap(void) {
    // icw1
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);

    // icw2
    // master: 0x20 = 32 - 0x28 = 40
    // irq0 -> irq7 = 32 -> 39
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);

    // icw3
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    // icw4
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    // Mask all IRQs (for now)
    outb(PIC1_DATA, 0xFC);
    outb(PIC2_DATA, 0xFF);
}
