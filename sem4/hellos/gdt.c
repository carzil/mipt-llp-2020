#include <stddef.h>
#include <stdint.h>

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  limit_high;
    uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct tss {
    uint32_t link;
    uint32_t esp0;
    uint16_t ss0;
    uint16_t reserved0;
    uint32_t reserved1[19];
} __attribute__((packed));

struct gdt_entry GDT[6];
struct gdt_ptr GDTPTR;
struct tss TSS;

#define ACCESS_PRESENT   0b10000000
#define ACCESS_RING0     0b00000000
#define ACCESS_RING3     0b01100000
#define ACCESS_SEGMENT   0b00010000
#define ACCESS_EXEC      0b00001000
#define ACCESS_RW        0b00000010
#define ACCESS_ACCESSED  0b00000001

static void gdt_register(int32_t idx, uint32_t base, uint32_t limit, uint8_t access) {
    GDT[idx].base_low = (base & 0xFFFF);
    GDT[idx].base_middle = (base >> 16) & 0xFF;
    GDT[idx].base_high = (base >> 24) & 0xFF;

    GDT[idx].limit_low = (limit & 0xFFFF);
    GDT[idx].limit_high = (limit >> 16) & 0x0F;

    GDT[idx].limit_high |= 0b11000000;
    GDT[idx].access = access | ACCESS_PRESENT;
}

uint8_t ring0_stack[16 * 1024];

extern void load_gdt(struct gdt_ptr*);

void init_gdt() {
    TSS.esp0 = (uint32_t)(&ring0_stack[0] + sizeof(ring0_stack));
    TSS.ss0 = 0x10;

    gdt_register(0, 0, 0, 0);
    gdt_register(1, 0, 0xffffffff, ACCESS_RING0 | ACCESS_SEGMENT | ACCESS_EXEC | ACCESS_RW);
    gdt_register(2, 0, 0xffffffff, ACCESS_RING0 | ACCESS_SEGMENT | ACCESS_RW);
    gdt_register(3, 0, 0xffffffff, ACCESS_RING3 | ACCESS_SEGMENT | ACCESS_EXEC | ACCESS_RW);
    gdt_register(4, 0, 0xffffffff, ACCESS_RING3 | ACCESS_SEGMENT | ACCESS_RW);
    gdt_register(5, (uint32_t)&TSS, sizeof(TSS) - 1, ACCESS_EXEC | 1);

    GDTPTR.base = (uint32_t)&GDT;
    GDTPTR.limit = sizeof(GDT) - 1;

    load_gdt(&GDTPTR);
}
