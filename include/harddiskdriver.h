#pragma once

#include <cstddef>

#include "harddiskdriver/HardDiskDriver.h"

void initHardDiskDriverModule();

HardDiskDriver* getHD(size_t i);