#include "keyboard.h"
#include <stdint.h>
#include <terminal.h>

void kb_handle_sc(uint8_t sc) {
    // ignore key release scancodes (TODO)
    if (sc & 0x80) {
        return;
    }

    switch (sc) {
    case 0x1E:
        terminal_putchar('a');
        break;

    case 0x30:
        terminal_putchar('b');
        break;

    case 0x2E:
        terminal_putchar('c');
        break;

    case 0x20:
        terminal_putchar('d');
        break;

    case 0x12:
        terminal_putchar('e');
        break;

    case 0x21:
        terminal_putchar('f');
        break;

    case 0x22:
        terminal_putchar('g');
        break;

    case 0x23:
        terminal_putchar('h');
        break;

    case 0x17:
        terminal_putchar('i');
        break;

    case 0x24:
        terminal_putchar('j');
        break;

    case 0x25:
        terminal_putchar('k');
        break;

    case 0x26:
        terminal_putchar('l');
        break;

    case 0x32:
        terminal_putchar('m');
        break;

    case 0x31:
        terminal_putchar('n');
        break;

    case 0x18:
        terminal_putchar('o');
        break;

    case 0x19:
        terminal_putchar('p');
        break;

    case 0x10:
        terminal_putchar('q');
        break;

    case 0x13:
        terminal_putchar('r');
        break;

    case 0x1F:
        terminal_putchar('s');
        break;

    case 0x14:
        terminal_putchar('t');
        break;

    case 0x16:
        terminal_putchar('u');
        break;

    case 0x2F:
        terminal_putchar('v');
        break;

    case 0x11:
        terminal_putchar('w');
        break;

    case 0x2D:
        terminal_putchar('x');
        break;

    case 0x15:
        terminal_putchar('y');
        break;

    case 0x2C:
        terminal_putchar('z');
        break;

    case 0x39:
        terminal_putchar(' ');
        break;

    case 0x1C:
        terminal_putchar('\n');
        break;

    default:
        break;
    }
}
