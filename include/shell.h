#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>

enum shell_key {
    SHELL_KEY_CHAR = 0,
    SHELL_KEY_LEFT,
    SHELL_KEY_RIGHT,
    SHELL_KEY_DELETE
};

void shell_initialize(void);
void shell_handle_char(char c);
void shell_handle_key(enum shell_key key);

#endif
