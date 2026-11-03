#include "keyboard.h"
#include "shell.h"
#include <stdint.h>
#include <terminal.h>

static const char scancode_table[128] = {
    [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4', [0x06] = '5',
    [0x07] = '6', [0x08] = '7', [0x09] = '8', [0x0A] = '9', [0x0B] = '0',

    [0x10] = 'q', [0x11] = 'w', [0x12] = 'e', [0x13] = 'r', [0x14] = 't',
    [0x15] = 'y', [0x16] = 'u', [0x17] = 'i', [0x18] = 'o', [0x19] = 'p',

    [0x1E] = 'a', [0x1F] = 's', [0x20] = 'd', [0x21] = 'f', [0x22] = 'g',
    [0x23] = 'h', [0x24] = 'j', [0x25] = 'k', [0x26] = 'l',

    [0x2C] = 'z', [0x2D] = 'x', [0x2E] = 'c', [0x2F] = 'v', [0x30] = 'b',
    [0x31] = 'n', [0x32] = 'm',

    [0x39] = ' ',
};

static const char shifted_scancode_table[128] = {
    [0x02] = '!', [0x03] = '@', [0x04] = '#', [0x05] = '$', [0x06] = '%',
    [0x07] = '^', [0x08] = '&', [0x09] = '*', [0x0A] = '(', [0x0B] = ')',

    [0x0C] = '_', [0x0D] = '+',

    [0x10] = 'Q', [0x11] = 'W', [0x12] = 'E', [0x13] = 'R', [0x14] = 'T',
    [0x15] = 'Y', [0x16] = 'U', [0x17] = 'I', [0x18] = 'O', [0x19] = 'P',

    [0x1A] = '{', [0x1B] = '}',

    [0x1E] = 'A', [0x1F] = 'S', [0x20] = 'D', [0x21] = 'F', [0x22] = 'G',
    [0x23] = 'H', [0x24] = 'J', [0x25] = 'K', [0x26] = 'L',

    [0x27] = ':', [0x28] = '"',

    [0x29] = '~',

    [0x2B] = '|',

    [0x2C] = 'Z', [0x2D] = 'X', [0x2E] = 'C', [0x2F] = 'V', [0x30] = 'B',
    [0x31] = 'N', [0x32] = 'M',

    [0x33] = '<', [0x34] = '>', [0x35] = '?',
};

static int shift_pressed = 0; // flag for if shift is held down
static int caps_lock = 0;

static int extended_scancode = 0; // extended scancodes dont arrive as 1 byte.
// Example: Left key -> E0 4B

void kb_handle_sc(uint8_t sc) {
    if (sc == 0xE0) {
        extended_scancode = 1;
        return;
    }

    if (extended_scancode) {
        extended_scancode = 0;

        switch (sc) {
        case 0x4B:
            shell_handle_key(SHELL_KEY_LEFT);
            return;

        case 0x4D:
            shell_handle_key(SHELL_KEY_RIGHT);
            return;

        case 0x53:
            shell_handle_key(SHELL_KEY_DELETE);
            return;

        default:
            return;
        }
    }

    if (sc == 0x0E) { // backspace
        shell_handle_char('\b');
    }

    // TODO: convert to switch
    if (sc == 0x3A) { // caps lock
        caps_lock = !caps_lock;
        return;
    }

    if (sc == 0x2A || sc == 0x36) { // left shift press
        shift_pressed = 1;
        return;
    }

    if (sc == 0xAA || sc == 0xB6) { // left shift release
        shift_pressed = 0;
        return;
    }

    if (sc == 0x1C) { // enter key
        shell_handle_char('\n');
        return;
    }

    if (sc == 0x0F) { // tab key
        shell_handle_char('\t');
        return;
    }

    if (sc & 0x80) {
        return;
    }

    char c;

    if (shift_pressed) {
        c = shifted_scancode_table[sc];

        if (c == 0) {
            c = scancode_table[sc];
        }
    } else {
        c = scancode_table[sc];
    }

    if (c == 0) {
        return;
    }

    if (!shift_pressed && caps_lock && c >= 'a' && c <= 'z') {
        c = c - 'a' + 'A';
    }

    if (shift_pressed && caps_lock && c >= 'A' && c <= 'Z') {
        c = c - 'A' + 'a';
    }

    shell_handle_char(c);
}
