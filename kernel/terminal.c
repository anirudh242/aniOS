#include "terminal.h"
#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_DEFAULT_COLOR 0x0F00

// VGA textmode buffer address
volatile unsigned short *video = (unsigned short *)0xB8000;

static int row;
static int col;

void terminal_initialize(void) {
    row = 0;
    col = 0;
}

void terminal_scroll(void) {
    for (size_t r = 0; r < VGA_HEIGHT; r++) {
        for (size_t c = 0; c < VGA_WIDTH; c++) {
            // copying row -> (row - 1)
            video[(r - 1) * VGA_WIDTH + c] = video[r * VGA_WIDTH + c];
        }
    }

    for (size_t c = 0; c < VGA_HEIGHT; c++) {
        // make last row completely blank
        video[(VGA_HEIGHT - 1) + c * VGA_WIDTH] = VGA_DEFAULT_COLOR | ' ';
    }
    row = VGA_HEIGHT - 1;
}

void terminal_putchar(char c) {
    // handling breakline
    if (c == '\n') {
        col = 0;
        row++;
        if (row >= VGA_HEIGHT) {
            terminal_scroll();
        }
        return;
    }

    video[row * VGA_WIDTH + col] = VGA_DEFAULT_COLOR | c;
    col++;

    // line wrapping
    if (col >= VGA_WIDTH) {
        col = 0;
        row++;
        if (row >= VGA_HEIGHT) {
            terminal_scroll();
        }
    }
}

void terminal_write(const char *str) {
    while (*str) {
        terminal_putchar(*str);
        str++;
    }
}

void terminal_write_hex(uint64_t val) {
    const char *hex = "0123456789ABCDEF";
    terminal_write("0x");
    // one hex digit = 4 bits so 16 iterations for uint64
    for (int i = 15; i >= 0; i--) {
        // rshift in multiples of 4 then keep last 4 bits (& 0XF)
        uint8_t digit = (val >> (i * 4)) & 0xF;
        terminal_putchar(hex[digit]);
    }
}
