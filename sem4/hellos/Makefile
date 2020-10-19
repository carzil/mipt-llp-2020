AS=gcc -m32 -c -g -mgeneral-regs-only -mno-red-zone
CC=gcc -m32 -g -mgeneral-regs-only -mno-red-zone -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LD=gcc -m32

build:
	$(AS) boot.s -o boot.o
	$(CC) -c kernel.c -o kernel.o
	$(CC) -c idt.c    -o idt.o
	$(CC) -c gdt.c    -o gdt.o
	$(AS) -c gdt.s    -o gdt_asm.o
	$(CC) -c acpi.c   -o acpi.o
	$(CC) -c apic.c   -o apic.o
	$(CC) -c vga.c    -o vga.o
	$(LD) -T linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib acpi.o gdt_asm.o gdt.o boot.o idt.o kernel.o vga.o apic.o -lgcc

clean:
	rm -f *.o
	rm -f kernel.bin

.PHONY: build clean
