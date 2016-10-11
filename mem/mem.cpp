#include "mem.h"

#include <new>

SystemMemoryManager memoryManager;
PhysicalMemoryManager physicalMemoryManager;
BuddyManager buddyManager;


void initMemModule()
{
    new (&physicalMemoryManager) PhysicalMemoryManager();
    new (&memoryManager) SystemMemoryManager();
    new (&buddyManager) BuddyManager();

    physicalMemoryManager.init();
    memoryManager.init();
    buddyManager.init();
}
