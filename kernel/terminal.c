#include "terminal.h"
#include "io.h"
#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_DEFAULT_COLOR 0x0F00
#define VGA_INDEX_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5

// VGA textmode buffer address
volatile unsigned short *video = (unsigned short *)0xB8000;

static int row;
static int col;

void terminal_initialize(void) {
    row = 0;
    col = 0;
}

void terminal_scroll(void) {
    for (size_t r = 1; r < VGA_HEIGHT; r++) {
        for (size_t c = 0; c < VGA_WIDTH; c++) {
            // copying row -> (row - 1)
            video[(r - 1) * VGA_WIDTH + c] = video[r * VGA_WIDTH + c];
        }
    }

    for (size_t c = 0; c < VGA_WIDTH; c++) {
        video[(VGA_HEIGHT - 1) * VGA_WIDTH + c] = VGA_DEFAULT_COLOR | ' ';
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

void terminal_backspace(void) {
    if (col > 0) {
        col--;
    } else if (row > 0) {
        row--;
        col = VGA_WIDTH - 1;
    } else {
        return;
    }

    video[row * VGA_WIDTH + col] = VGA_DEFAULT_COLOR | ' ';
}

void terminal_get_cursor(size_t *out_row, size_t *out_col) {
    *out_row = row;
    *out_col = col;
}

void terminal_redraw_line(
    size_t start_row, size_t start_col, const char *str, size_t length,
    size_t old_length,
    size_t cursor_position) { // fixes cursor bug where moving cursor and
                              // editing a line results in character overwriting
                              // other characters
    for (size_t i = 0; i < length; i++) {
        video[start_row * VGA_WIDTH + start_col + i] =
            VGA_DEFAULT_COLOR | str[i];
    }

    for (size_t i = length; i < old_length; i++) {
        video[start_row * VGA_WIDTH + start_col + i] = VGA_DEFAULT_COLOR | ' ';
    }

    row = start_row;
    col = start_col + cursor_position;

    terminal_update_cursor();
}

void terminal_cursor_left(void) {
    if (col > 0) {
        col--;
    }
}

void terminal_cursor_right(void) {
    if (col < VGA_WIDTH - 1) {
        col++;
    }
}

static void terminal_update_cursor(void) {
    uint16_t position = row * VGA_WIDTH + col;

    outb(VGA_INDEX_PORT, 0x0F);
    outb(VGA_DATA_PORT, position & 0xFF);

    outb(VGA_INDEX_PORT, 0x0E);
    outb(VGA_DATA_PORT, (position >> 8) & 0xFF);
}
