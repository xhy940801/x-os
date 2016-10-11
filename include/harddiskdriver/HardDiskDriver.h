#pragma once

#include "blockdriver.h"

class RealHardDiskDriver;

class HardDiskDriver : public BlockDriver
{
    bool initialized;
    size_t start;
    size_t len;
    RealHardDiskDriver* driver;

public:
    HardDiskDriver() : initialized(false) {}

    void init(size_t _start, size_t _len, RealHardDiskDriver* _driver)
    {
        start = _start;
        len = _len;
        driver = _driver;
        initialized = true;
    }

    virtual int read(size_t lba, char* buffers[], size_t count) override;
    virtual int read(size_t lba, char* buffers[], size_t count, long timeout) override;

    virtual int write(size_t lba, char* buffers[], size_t count) override;
    virtual int write(size_t lba, char* buffers[], size_t count, long timeout) override;
};