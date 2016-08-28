#include "mem.h"

SystemMemoryManager memoryManager;
PhysicalMemoryManager physicalMemoryManager;
BuddyManager buddyManager;


void initMemModule()
{
	physicalMemoryManager.init();
    memoryManager.init();
    buddyManager.init();
}
