#include "driver.hpp"

#define modeinfo_section __attribute__((used)) __attribute__((section(".modinfo"))) __attribute__((aligned(1)))
#define module_section __attribute__((used)) __attribute__((section(".gnu.linkonce.this_module"))) __attribute__((aligned(64)))
#define versions_section __attribute__((used)) __attribute__((section("__versions"))) __attribute__((aligned(32)))

modeinfo_section static const char name[] = "name=iDriver";
modeinfo_section static const char author[] = "author=Alexander U.Jurankov <heir@mail.ru> MakeDUMP";
modeinfo_section static const char description[] = "description=Universal PCIe Driver";
modeinfo_section static const char license[] = "license=GPL v2";
modeinfo_section static const char retpoline[] = "retpoline=Y";
versions_section static const char versions[] = "\x00\x00\x00\x00\x00\x00\x00\x00";

extern "C" int init_module() { return driver_init(); }
extern "C" void cleanup_module(void) { driver_clean(); }

// linux 6.3.0 = 896 bytes
// RELOCATION RECORDS FOR [.gnu.linkonce.this_module]:
// OFFSET           TYPE              VALUE
// 0000000000000138 R_X86_64_64       init_module
// 0000000000000338 R_X86_64_64       cleanup_module

// linux 6.5.0 = 1280 bytes
// RELOCATION RECORDS FOR [.gnu.linkonce.this_module]:
// OFFSET           TYPE                     VALUE
// 0000000000000138 R_X86_64_64              init_module
// 0000000000000490 R_X86_64_64              cleanup_module

#if defined(linux_630)
modeinfo_section static const char vermagic[] = "vermagic=6.3.0-generic SMP preempt mod_unload modversions ";
constexpr auto this_module_section_size = 896;
constexpr auto cleanup_offset = 824; // 0x338
#else
modeinfo_section static const char vermagic[] = "vermagic=6.5.0-generic SMP preempt mod_unload modversions ";
constexpr auto this_module_section_size = 1280;
constexpr auto cleanup_offset = 1168; // 0x490
#endif

module_section extern "C" struct {
    estd::uint8_t reserved_1[24];
    char name[64 - sizeof(unsigned long)]; // 56 для 64-x бит
    estd::uint8_t reserved_2[232];
    int (*init)(void); // Startup function

    estd::uint8_t reserved_3[(cleanup_offset - sizeof(reserved_1) - sizeof(name) - sizeof(reserved_2) - sizeof(init))];
    void (*exit)(void); // Destruction function
    estd::uint8_t reserved_4[this_module_section_size - cleanup_offset - sizeof(exit)];
} __attribute__((__packed__)) __this_module = {
    .name = "iDriver",
    .init = init_module,
    .exit = cleanup_module
};
