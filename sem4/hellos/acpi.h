#pragma once

#include <stddef.h>
#include <stdint.h>

struct acpi_sdt_header {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    char oem_tableid[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed));

struct acpi_sdt {
    struct acpi_sdt_header header;

    union {
        struct acpi_sdt* entries[0];
        char data[0];
    };

} __attribute__((packed));


struct acpi_rsdp {
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdt_addr;
} __attribute__ ((packed));

struct acpi_sdt* acpi_find_rsdt();
struct acpi_sdt* acpi_find_sdt(struct acpi_sdt* root, const char* signature);
