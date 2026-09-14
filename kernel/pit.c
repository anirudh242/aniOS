#include "pit.h"
#include "io.h"
#include <stdint.h>

#define PIT_COMMAND 0x43
#define PIT_CHANNEL0 0x40
#define PIT_BASE_FREQUENCY 1193182

void pit_init(uint32_t freq) {
    if (freq == 0) {
        return;
    }

    uint16_t divisor = PIT_BASE_FREQUENCY / freq;

    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
}
