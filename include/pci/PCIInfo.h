#pragma once

#include <cstdint>

struct PCIInfo
{
    uint16_t vendorID, deviceID;
    uint16_t command, status;
    uint8_t revisionID, progIF, subclass, classCode;
    uint8_t cacheLineSize, latencyTimer, headerType, bist;
    uint32_t bar0;
    uint32_t bar1;
    uint32_t bar2;
    uint32_t bar3;
    uint32_t bar4;
    uint32_t bar5;
    uint32_t cardbusCISPointer;
    uint16_t subsystemVendorID, subsystemID;
    uint32_t expansionRomBaseAddress;
    uint8_t capabilitiesPointer, reserved0, reserved1, reserved2;
    uint32_t reserved3;
    uint8_t interruptLine, interruptPin, minGrant, maxLatency;
};