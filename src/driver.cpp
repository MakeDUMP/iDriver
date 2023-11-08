#include "driver.hpp"

estd::pair<estd::uintptr_t, bool> map_physical_memory(estd::size_t start, estd::size_t length)
{
    // print_info("map_physical_memory: Physical addr: 0x%" PRIX64 ", Size: 0x%" PRIX64, start, length);
#if defined(_WIN32)
    auto addr = MmMapIoSpace(start, length, /* MmNonCached */ 0);
#else
    auto addr = pat_enabled() ? ioremap_uc(start, length) : ioremap(start, length);
#endif

    if (addr) {
        // print_info("map_physical_memory: Linear addr: 0x%" PRIX64, addr);
        return estd::make_pair(addr, false);
    } else {
        print_error("map_physical_memory: Failed to translate addr: 0x%" PRIX64, start);
        return estd::make_pair(estd::uintptr_t(nullptr), true);
    }
}

void unmap_physical_memory(estd::uintptr_t addr, estd::size_t length)
{
    // print_info("unmap_physical_memory: Linear addr: 0x%" PRIX64 ", Size: 0x%" PRIX64, addr, length);
#if defined(_WIN32)
    MmUnmapIoSpace(addr, length);
#else
    (void)length; // disabled warning: unused parameter
    iounmap(addr);
#endif
}
