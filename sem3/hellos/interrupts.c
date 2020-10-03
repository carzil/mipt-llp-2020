#include <stddef.h>
#include <stdint.h>

#include "interrupts.h"

struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry IDT[256];
struct idt_ptr IDTPTR;

static void idt_register(uint8_t pos, uint32_t base, uint16_t sel, uint8_t flags)
{
    IDT[pos].base_lo = base & 0xFFFF;
    IDT[pos].base_hi = (base >> 16) & 0xFFFF;

    IDT[pos].sel = sel;
    IDT[pos].zero = 0;
    IDT[pos].flags = flags;
}

extern void isr0();

void init_idt() {
    IDTPTR.limit = sizeof(struct idt_entry) * 256 - 1;
    IDTPTR.base = (uint32_t)&IDT;

    // For flags param see https://wiki.osdev.org/IDT#Structure_IA-32.
    idt_register(0, (uint32_t)isr0, 0x08, 0b10001110);
    idt_register(0x80, (uint32_t)isr0, 0x08, 0b10001110);

    asm volatile (
        "lidt (%0)\n"
        :
        : "r"(&IDTPTR)
        :
    );
}
