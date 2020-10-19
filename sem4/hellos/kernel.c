#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "idt.h"
#include "gdt.h"
#include "acpi.h"
#include "apic.h"
#include "vga.h"
#include "panic.h"
#include "spinlock.h"


__attribute__ ((interrupt)) void syscall_entry(struct iframe* frame) {
    terminal_writestring_color("Syscall!\n", vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
    (void)frame;
}

static int lock = 0;

__attribute__ ((interrupt)) void timer_isr(struct iframe* frame) {
    (void)frame;

    spin_lock(&lock);
    terminal_writestring_color(".", vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK));
    spin_unlock(&lock);

    apic_eoi();
}

__attribute__ ((interrupt)) void keyboard_isr(struct iframe* frame) {
    (void)frame;

    spin_lock(&lock);
    terminal_writestring_color(".", vga_entry_color(VGA_COLOR_RED, VGA_COLOR_GREEN));
    spin_unlock(&lock);

    apic_eoi();
}

extern void jump_userspace();

void kernel_main(void) {
    init_gdt();
    init_idt();

	terminal_initialize();
    terminal_writestring_color("HeLL OS is loaded.\n", vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
    struct acpi_sdt* rsdt = acpi_find_rsdt();
    if (!rsdt) {
        panic("RSDT not found!");
    }

    apic_init(rsdt);

    asm ("sti");
    // jump_userspace();
}
