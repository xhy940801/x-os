#include "pci.h"

#include "pci/commondefs.h"

#include "asm.h"

void initPCIModule()
{
}

void loadPCIConfigurationSpace(PCIInfo* pciInfo, uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t lbus  = bus;
    uint32_t lslot = slot;
    uint32_t lfunc = func;
    uint32_t address = static_cast<uint32_t>((lbus << 16) | (lslot << 11) |
            (lfunc << 8) | (offset) | ((uint32_t)0x80000000));
    uint32_t* data = reinterpret_cast<uint32_t*>(pciInfo);
    for(size_t i = 0; i < sizeof(*pciInfo) / sizeof(uint32_t); ++i)
    {
        _outd(pci::PCI_CONFIG_ADDRESS, address | (i * sizeof(uint32_t)));
        data[i] = _ind(pci::PCI_CONFIG_DATA);
    }
}