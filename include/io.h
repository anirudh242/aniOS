#ifndef IO_H
#define IO_H

#include <stdint.h>

void outb(uint16_t port, uint8_t value); // send 1 byte to x86 IO port
uint8_t inb(uint16_t port);              // read 1 byte from x86 IO port

#endif
