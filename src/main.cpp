#include "driver.hpp"

int driver_init()
{
    print_info("--=[ INIT ]=-------------------------------------------------------------------");

    auto pciconf_base_address = get_pciconf_base_address();
    if (pciconf_base_address == 0)
        return -1;

    return 0;
}

void driver_clean()
{
    print_info("--=[ CLEAN ]=------------------------------------------------------------------");
}
