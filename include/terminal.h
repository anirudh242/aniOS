#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <stdint.h>

void terminal_initialize(void);

void terminal_putchar(char c);
void terminal_write(const char *str);
void terminal_write_hex(uint64_t val);

void terminal_backspace(void);

static void terminal_update_cursor(); // VGA cursor implementation
void terminal_get_cursor(size_t *out_row, size_t *out_col);
void terminal_cursor_left(void);
void terminal_cursor_right(void);

void terminal_redraw_line(size_t start_row, size_t start_col, const char *str,
                          size_t length, size_t old_length,
                          size_t cursor_position);

#endif
