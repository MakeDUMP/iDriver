#include "driver.hpp"

constexpr auto IRP_MJ_MAXIMUM_FUNCTION = 0x1B;

struct UNICODE_STRING {
    estd::uint16_t Length;
    estd::uint16_t MaximumLength;
    estd::uint16_t const* Buffer;
};

struct DRIVER_OBJECT {
    estd::uint16_t Type;
    estd::uint16_t Size;
    estd::uintptr_t DeviceObject;
    estd::uint32_t Flags;
    estd::uintptr_t DriverStart;
    estd::uint32_t DriverSize;
    estd::uintptr_t DriverSection;
    estd::uintptr_t DriverExtension;
    UNICODE_STRING DriverName;
    UNICODE_STRING const* HardwareDatabase;
    estd::uintptr_t FastIoDispatch;
    estd::uintptr_t DriverInit;
    estd::uintptr_t DriverStartIo;
    estd::uintptr_t DriverUnload;
    estd::uintptr_t MajorFunction[IRP_MJ_MAXIMUM_FUNCTION + 1];
};

void DriverUnload(DRIVER_OBJECT& /*driver*/)
{
    // print_info("DriverUnload():");
    // print_info("    DriverObject = 0x%p", &driver);

    driver_clean();
}

extern "C" int DriverEntry(DRIVER_OBJECT& driver, UNICODE_STRING& /*path*/)
{
    // print_info("DriverEntry():");
    // print_info("    DriverObject = 0x%p", &driver);
    // print_info("    DriverName = %ws", driver.DriverName.Buffer);
    // print_info("    DriverNameLength = %d", driver.DriverName.Length);
    // print_info("    Size = %d | %d", driver.Size, sizeof(DRIVER_OBJECT));

    driver.DriverUnload = estd::uintptr_t(DriverUnload);

    return driver_init();
}
