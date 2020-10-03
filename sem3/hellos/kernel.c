#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "interrupts.h"

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len]) {
		len++;
    }
	return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint16_t* terminal_buffer;

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	uint8_t color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', color);
		}
	}
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar_color(char c, uint8_t color) {
    switch (c) {
    case '\n':
        terminal_row++;
        terminal_column = 0;
        if (terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
        break;

    default:
        terminal_putentryat(c, color, terminal_column, terminal_row);
        terminal_column++;
        if (terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            terminal_row++;
            if (terminal_row == VGA_HEIGHT) {
                terminal_row = 0;
            }
        }
    }
}

void terminal_write(const char* data, size_t size, uint8_t color) {
	for (size_t i = 0; i < size; i++) {
        terminal_putchar_color(data[i], color);
    }
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data), vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

void terminal_writestring_color(const char* data, uint8_t color) {
    terminal_write(data, strlen(data), color);
}


__attribute__ ((interrupt)) void isr0(struct iframe* frame) {
    terminal_writestring_color("Hello world!\n", vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
    (void)frame;
}

void kernel_main(void) {
    init_idt();

	terminal_initialize();
    asm volatile ("sti");
    asm volatile ("int $0x80");

    terminal_writestring_color("(c) carzil", vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
}
