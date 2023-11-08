#include "driver.hpp"

#include <string_view.hpp>
using namespace estd::literals::string_literals;

struct RSDP { // Root System Description Table Pointer Structure
    estd::uint8_t signature[8]; // signature 'RSD PTR '
    estd::uint8_t checksum; // checksum for first 20 bytes of table (entire ACPI 1.0 table)
    estd::uint8_t oem_id[6]; // String that uniquely ID's the OEM
    estd::uint8_t revision; // 0 - ACPI 1.0;  2 - ACPI 2.0
    estd::uint32_t rsdt_address; // physical address of Root System Description Table (1.0 table ended here)
    estd::uint32_t length; // Length of the table in bytes
    estd::uint64_t xsdt_address; // physical address of XSDT
    estd::uint8_t xchecksum; // checksum for entire table
    estd::uint8_t reserved[3];
} __attribute__((__packed__));

struct SDT {
    estd::uint8_t signature[4];
    estd::uint32_t length;
    estd::uint8_t revision;
    estd::uint8_t checksum;
    estd::uint8_t oem_id[6];
    estd::uint8_t oem_table_id[8];
    estd::uint32_t oem_revision;
    estd::uint32_t creator_id;
    estd::uint32_t creator_revision;
} __attribute__((__packed__));

struct xSDT {
    SDT sdt;
    estd::uint64_t pointer_to_other_sdt[];
} __attribute__((__packed__));

estd::pair<estd::size_t, bool> acpi_scan_rsdp(estd::size_t start, estd::size_t length)
{
    auto [addr, error] = map_physical_memory(start, length);
    if (error)
        return estd::make_pair(estd::uintptr_t(nullptr), true);

    estd::string_view buffer { reinterpret_cast<char const*>(addr), length };
    auto offset = buffer.find("RSD PTR "sv); // signature 'RSD PTR '

    unmap_physical_memory(addr, length);
    return estd::make_pair(start + offset, (offset == estd::string_view::npos) ? true : false);
}

estd::uintptr_t get_pciconf_base_address()
{
    print_info("get_pciconf_base_address: Searching to locate RSDP");

    // ищем "RSDP" в области первого килобайта (расширенная обрасть данных BIOS)
    auto [offset, error] = acpi_scan_rsdp(0x00000, 0x00400);
    if (error) {
        // ищем "RSDP" в области памяти от 0xE0000 до 0xFFFFF (основная область BIOS ниже первого мегабайта)
        auto [offset_new, error] = acpi_scan_rsdp(0xE0000, 0x20000);
        if (error) {
            print_error("get_pciconf_base_address: Unable to locate RSDP");
            return {};
        }
        offset = offset_new;
    }
    print_info("get_pciconf_base_address: Found to locate RSDP offset = 0x%llX", offset);

    auto [addr, error2] = map_physical_memory(offset, sizeof(RSDP));
    if (error2)
        return {};

    auto rsdp = reinterpret_cast<RSDP*>(addr);
    print_info("get_pciconf_base_address: ACPI Revision: %d, OEM ID: \"%c%c%c%c%c%c\"", rsdp->revision,
        rsdp->oem_id[0], rsdp->oem_id[1], rsdp->oem_id[2], rsdp->oem_id[3], rsdp->oem_id[4], rsdp->oem_id[5]);

    // проверяем версию таблицы ACPI
    if (rsdp->revision < 2) {
        print_error("get_pciconf_base_address: required ACPI Specification v2.0 or higher");
        return {};
    }

    // проверяем контрольную сумму
    estd::uint8_t checksum {};
    for (estd::size_t i = 0; i < rsdp->length; i++)
        checksum += reinterpret_cast<estd::uint8_t*>(addr)[i];
    if (checksum)
        print_info("get_pciconf_base_address: Invalid RSDP checksum");

    // получаем адрес таблицы XSDT
    auto xsdt_address = rsdp->xsdt_address;
    unmap_physical_memory(addr, sizeof(RSDP));

    // поиск таблицы MCFG
    print_info("get_pciconf_base_address: Searching MCFG in XSDT: 0x%llX", xsdt_address);

    auto [sdt_mem, error_sdt] = map_physical_memory(xsdt_address, sizeof(SDT));
    if (error_sdt)
        return {};
    auto xsdt_length = reinterpret_cast<SDT*>(sdt_mem)->length;
    unmap_physical_memory(xsdt_address, sizeof(SDT));

    auto [xsdt_mem, error_xsdt] = map_physical_memory(xsdt_address, xsdt_length);
    if (error_xsdt)
        return {};
    auto xsdt = reinterpret_cast<xSDT*>(xsdt_mem);

    estd::uintptr_t mcfg_offset {};
    for (estd::size_t i = 0; i < (xsdt->sdt.length - sizeof(SDT)) / sizeof(estd::uint64_t); i++) {
        auto [sdt_mem, error_sdt] = map_physical_memory(xsdt->pointer_to_other_sdt[i], sizeof(SDT));
        if (error_sdt)
            return {};
        auto sdt = reinterpret_cast<SDT*>(sdt_mem);
        estd::string_view sign { reinterpret_cast<char const*>(sdt->signature), sizeof(sdt->signature) };

        if (sign == "MCFG"sv)
            mcfg_offset = xsdt->pointer_to_other_sdt[i] + 44 * sizeof(estd::uint32_t);
        unmap_physical_memory(xsdt->pointer_to_other_sdt[i], sizeof(SDT));

        if (mcfg_offset != 0)
            break;
    }

    unmap_physical_memory(xsdt_address, xsdt_length);

    if (mcfg_offset == 0) {
        print_error("get_pciconf_base_address: MCFG table not found");
        return {};
    }

    print_info("get_pciconf_base_address: Found PCI configuration base address: 0x%llX", mcfg_offset);
    return mcfg_offset;
}
