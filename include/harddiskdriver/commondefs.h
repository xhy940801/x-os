#pragma once

#include <cstddef>

namespace harddiskdriver
{
    const size_t OPERATION_CAPACITY = 32;
    const size_t PORT_DATA = 0x0000;
    const size_t PORT_ERROR = 0x0001;
    const size_t PORT_SELECTORCOUNT = 0x0002;
    const size_t PORT_LBALO = 0x0003;
    const size_t PORT_LBAMID = 0x0004;
    const size_t PORT_LBAHI = 0x0005;
    const size_t PORT_DRIVERSELECT = 0x0006;
    const size_t PORT_CMDSTATUS = 0x0007;
    const size_t PORT_DRIVER_CTL = 0x0206;

    const size_t PORT_BMI_M_COMMAND = 0x0000;
    const size_t PORT_BMI_M_STATUS = 0x0002;
    const size_t PORT_BMI_M_PRDTADDR = 0x0004;
}