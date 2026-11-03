#include "shell.h"
#include "terminal.h"
#include <stddef.h>

#define SHELL_BUFFER_SIZE 256

static char input_buffer[SHELL_BUFFER_SIZE];
static size_t input_row;
static size_t input_col;
static size_t input_length = 0;

static size_t cursor_position = 0;

void shell_initialize(void) {
    input_length = 0;
    cursor_position = 0;

    terminal_write("> ");

    terminal_get_cursor(&input_row, &input_col);
}

static void shell_redraw_line(size_t old_length) {
    terminal_redraw_line(input_row, input_col, input_buffer, input_length,
                         old_length, cursor_position);
}

void shell_handle_key(enum shell_key key) {
    if (key == SHELL_KEY_LEFT) {
        if (cursor_position > 0) {
            cursor_position--;
            shell_redraw_line(input_length);
        }
        return;
    }

    if (key == SHELL_KEY_RIGHT) {
        if (cursor_position < input_length) {
            cursor_position++;
            shell_redraw_line(input_length);
        }
        return;
    }

    if (key == SHELL_KEY_DELETE) {
        if (cursor_position >= input_length) {
            return;
        }
        size_t old_length = input_length;

        for (size_t i = cursor_position; i < input_length - 1; i++) {
            input_buffer[i] = input_buffer[i + 1];
        }

        input_length--;

        shell_redraw_line(old_length);
        return;
    }
}

void shell_handle_char(char c) {
    if (c == '\n') {
        input_buffer[input_length] = '\0';

        terminal_putchar('\n');
        terminal_write("You typed: ");
        terminal_write(input_buffer);
        terminal_write("\n> ");

        input_length = 0;
        cursor_position = 0;

        terminal_get_cursor(&input_row, &input_col);
        return;
    }

    if (c == '\b') {
        if (cursor_position == 0) {
            return;
        }

        size_t old_length = input_length;

        for (size_t i = cursor_position - 1; i < input_length - 1; i++) {
            input_buffer[i] = input_buffer[i + 1];
        }

        input_length--;
        cursor_position--;

        shell_redraw_line(old_length);
        return;
    }

    if (input_length >= SHELL_BUFFER_SIZE - 1) {
        return;
    }

    size_t old_length = input_length;

    /*
     * Move everything after the cursor one position right.
     */
    for (size_t i = input_length; i > cursor_position; i--) {
        input_buffer[i] = input_buffer[i - 1];
    }

    input_buffer[cursor_position] = c;
    input_length++;
    cursor_position++;

    shell_redraw_line(old_length);
}
