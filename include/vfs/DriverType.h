#pragma once

#include <cstdint>

class DriverType
{
    union
    {
        struct
        {
            uint16_t _subdriver;
            uint16_t _maindriver;
        };
        uint32_t _drivertype;
    };
public:
    DriverType(uint16_t subdriver, uint16_t maindriver)
        : _subdriver(subdriver), _maindriver(maindriver)
    {
    }

    DriverType(uint32_t drivertype)
        : _drivertype(drivertype)
    {
    }

    uint16_t subdriver() const
    {
        return _subdriver;
    }

    uint16_t maindriver() const
    {
        return _maindriver;
    }

    uint32_t drivertype() const
    {
        return _drivertype;
    }
};
