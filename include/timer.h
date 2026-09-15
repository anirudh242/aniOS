#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void timer_tick(void); // increase timer tick
uint64_t timer_get_ticks(void);
void timer_sleep(uint64_t ticks);

#endif
