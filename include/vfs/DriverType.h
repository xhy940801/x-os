#pragma once

#include <cstdint>

enum class MainDriverType : uint16_t
{
    NONE    = 0,
    HD      = 3,
    TTY0    = 5
};

class DriverType
{
    union
    {
        struct
        {
            uint16_t _subdriver;
            MainDriverType _maindriver;
        };
        uint32_t _drivertype;
    };
public:
    DriverType(uint16_t subdriver, MainDriverType maindriver)
        : _subdriver(subdriver), _maindriver(maindriver)
    {
        static_assert(sizeof(*this) == 4, "sizeof(DriverType) must equal to 4");
    }

    DriverType(uint32_t drivertype)
        : _drivertype(drivertype)
    {
        static_assert(sizeof(*this) == 4, "sizeof(DriverType) must equal to 4");
    }

    uint16_t subdriver() const
    {
        return _subdriver;
    }

    MainDriverType maindriver() const
    {
        return _maindriver;
    }

    uint32_t drivertype() const
    {
        return _drivertype;
    }
};
