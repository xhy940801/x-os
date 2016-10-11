#pragma once

#include "pci/PCIInfo.h"

void initPCIModule();

void loadPCIConfigurationSpace(PCIInfo* pciInfo, uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);