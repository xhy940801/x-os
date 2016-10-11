#include "harddiskdriver/HardDiskDriver.h"

#include "harddiskdriver/RealHardDiskDriver.h"

#include "mem.h"

int HardDiskDriver::read(size_t lba, char* buffers[], size_t count)
{
    assert(lba % mem::PAGESIZE == 0);
    assert(count * mem::PAGESIZE + lba <= len);
    lba /= 512;
    return driver->readOnDMAMode(lba + start, buffers, count);
}

int HardDiskDriver::read(size_t lba, char* buffers[], size_t count, long timeout)
{
    assert(lba % mem::PAGESIZE == 0);
    assert(count * mem::PAGESIZE + lba <= len);
    lba /= 512;
    return driver->readOnDMAMode(lba + start, buffers, count, timeout);
}

int HardDiskDriver::write(size_t lba, char* buffers[], size_t count)
{
    assert(lba % mem::PAGESIZE == 0);
    assert(count * mem::PAGESIZE + lba <= len);
    lba /= 512;
    return driver->writeOnDMAMode(lba + start, buffers, count);
}

int HardDiskDriver::write(size_t lba, char* buffers[], size_t count, long timeout)
{
    assert(lba % mem::PAGESIZE == 0);
    assert(count * mem::PAGESIZE + lba <= len);
    lba /= 512;
    return driver->writeOnDMAMode(lba + start, buffers, count, timeout);
}
