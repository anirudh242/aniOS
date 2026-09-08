#include "terminal.h"
void kernel_main(void) {
    terminal_initialize();

    terminal_write("aniOS\n");
    terminal_write("64-bit kernel initialized");
}
