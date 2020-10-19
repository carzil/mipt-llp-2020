#pragma once

#include "acpi.h"

void apic_init(struct acpi_sdt* rsdt);
void apic_eoi();
