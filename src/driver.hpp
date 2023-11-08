#pragma once
#include <stdint.hpp>
#include <utility.hpp>

#define print_info_str "iDriver: [INFO] "
#define print_warning_str "iDriver: [WARNING] "
#define print_error_str "iDriver: [ERROR] "

// ----------------------------------------------------------------------------
// Common functions
// ----------------------------------------------------------------------------
int driver_init();
void driver_clean();

estd::pair<estd::uintptr_t, bool> map_physical_memory(estd::size_t, estd::size_t);
void unmap_physical_memory(estd::uintptr_t, estd::size_t);

estd::uintptr_t get_pciconf_base_address();

#if defined(_WIN32)
// ----------------------------------------------------------------------------
// Windows API
// ----------------------------------------------------------------------------
#define print_info(fmt, ...) DbgPrint(print_info_str fmt "\n", ##__VA_ARGS__)
#define print_warning(fmt, ...) DbgPrint(print_warning_str fmt "\n", ##__VA_ARGS__)
#define print_error(fmt, ...) DbgPrint(print_error_str fmt "\n", ##__VA_ARGS__)

extern "C" __declspec(dllimport) estd::uint32_t DbgPrint(char const* fmt, ...);
extern "C" __declspec(dllimport) estd::uintptr_t MmMapIoSpace(estd::uintptr_t, estd::size_t, estd::size_t);
extern "C" __declspec(dllimport) void MmUnmapIoSpace(estd::uintptr_t, estd::size_t);

#else
// ----------------------------------------------------------------------------
// Linux API
// ----------------------------------------------------------------------------
#define print_info(fmt, ...) _printk("\0016" print_info_str fmt "\n", ##__VA_ARGS__)
#define print_warning(fmt, ...) _printk("\0014" print_warning_str fmt "\n", ##__VA_ARGS__)
#define print_error(fmt, ...) _printk("\0013" print_error_str fmt "\n", ##__VA_ARGS__)

extern "C" int _printk(char const* fmt, ...);
extern "C" bool pat_enabled();
extern "C" estd::uintptr_t ioremap(estd::uintptr_t, estd::size_t);
extern "C" estd::uintptr_t ioremap_uc(estd::uintptr_t, estd::size_t);
extern "C" void iounmap(estd::uintptr_t);

#endif
