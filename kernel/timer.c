#include "timer.h"
#include <stdint.h>
static volatile uint64_t timer_ticks = 0;

void timer_tick(void) { timer_ticks++; }

uint64_t timer_get_ticks(void) { return timer_ticks; }

void timer_sleep(uint64_t ticks) {
    uint64_t sleep_till = timer_get_ticks() + ticks;

    // sleep till timer hits target ticks
    while (timer_get_ticks() < sleep_till) {
        asm volatile("hlt");
    }
}
