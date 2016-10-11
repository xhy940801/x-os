#pragma once

#include <cstdint>

struct DriverInfo
{
    union
    {
        struct
        {
            uint16_t subDriver;
            uint16_t mainDriver;
        };
        uint32_t driver;
    };

    DriverInfo() = default;
    DriverInfo(uint16_t sub, uint16_t main) : subDriver(sub), mainDriver(main) {}
    DriverInfo(uint32_t dri) : driver(dri) {}
};