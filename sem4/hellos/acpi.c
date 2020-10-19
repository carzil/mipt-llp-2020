#include "acpi.h"
#include "common.h"

static struct acpi_rsdp* find_rsdp_in_region(void* start, size_t len) {
    for (size_t i = 0; i < len - 8; i++) {
        void* addr = (uint8_t*)start + i;
        if (memcmp("RSD PTR ", (const char*)addr, 8) == 0) {
            return (struct acpi_rsdp*)addr;
        }
    }
    return NULL;
}

struct acpi_sdt* acpi_find_rsdt() {
    // 1KB of EBDA.
    void* ebda_addr = (void*)((*(uint16_t*)0x40e) << 4);
    struct acpi_rsdp* rsdp = find_rsdp_in_region(ebda_addr, 1024);
    if (!rsdp) {
        // Static memory region.
        rsdp = find_rsdp_in_region((void*)0xe0000, 0xfffff - 0xe0000);
    }

    if (!rsdp) {
        return NULL;
    }
    return (struct acpi_sdt*)rsdp->rsdt_addr;
}

struct acpi_sdt* acpi_find_sdt(struct acpi_sdt* root, const char* signature) {
    size_t sz = (root->header.length - sizeof(root->header)) / 4;
    for (size_t i = 0; i < sz; i++) {
        if (memcmp(signature, &root->entries[i]->header.signature, 4) == 0) {
            return root->entries[i];
        }
    }
    return NULL;
}
