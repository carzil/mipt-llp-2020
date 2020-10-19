#pragma once

#include "vga.h"

static inline void panic(const char* msg) {
    terminal_writestring_color("PANIC: ", vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
    terminal_writestring_color(msg, vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
    for (;;);
}
