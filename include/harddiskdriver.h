#pragma once

#include <cstddef>

#include "harddiskdriver/HardDiskDriver.h"
#include "harddiskdriver/RealHardDiskDriver.h"

extern RealHardDiskDriver hddriver0;

void initHardDiskDriverModule();

HardDiskDriver* getHD(size_t i);