#pragma once

#include "mem/SystemMemoryManager.h"
#include "mem/BuddyManager.h"
#include "mem/PhysicalMemoryManager.h"

extern SystemMemoryManager memoryManager;
extern PhysicalMemoryManager physicalMemoryManager;
extern BuddyManager buddyManager;

void initMemModule();
