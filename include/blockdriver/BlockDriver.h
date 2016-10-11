#pragma once

#include <cstddef>
#include <cstdint>

#include "blockdriver/DriverInfo.h"
#include "blockdriver/commondefs.h"

class BlockDriver
{
    DriverInfo info;
public:
    virtual int read(size_t lba, char* buffers[], size_t count) = 0;
    virtual int read(size_t lba, char* buffers[], size_t count, long timeout) = 0;

    virtual int write(size_t lba, char* buffers[], size_t count) = 0;
    virtual int write(size_t lba, char* buffers[], size_t count, long timeout) = 0;
};