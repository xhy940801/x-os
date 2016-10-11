#pragma once

#include "List.h"
#include "multiproc/Semaphore.h"

#include "blockdriver/commondefs.h"

#include "harddiskdriver/commondefs.h"
#include "harddiskdriver/HardDiskDriver.h"
#include "harddiskdriver/HardDiskOperationInfo.h"
#include "harddiskdriver/HardDiskElevator.h"

class PCIInfo;

class RealHardDiskDriver
{
    struct PRDTInfo
    {
        uint32_t physicalAddress;
        uint16_t byteCount;
        uint16_t eot;
    };

    struct BMRInfo
    {
        int basePort;
        uintptr_t prdtPhysicalAddress;
        PRDTInfo* prdt;
    };

private:
    int basePort;
    int slaveBit;
    size_t len;
    Semaphore sem;
    HardDiskOperationInfo operations[harddiskdriver::OPERATION_CAPACITY];
    List<HardDiskOperationInfo, 0> freeList;
    BMRInfo bmr;
    HardDiskDriver partitions[4];
    HardDiskElevator elevator;

private:
    void read256word(uint16_t* data);
    void write256word(const uint16_t* data);
    void resetPartitionTable();
    void initPartitionTable();
    void initBMR();
    bool enumeratingATAController(PCIInfo* info);

    void processOneOperation();

    int readOnDMAModeImmediately(size_t lba, char* buffers[], uint16_t count);
    int writeOnDMAModeImmediately(size_t lba, char* buffers[], uint16_t count);

public:
    RealHardDiskDriver(int _basePort, int _slaveBit);

    RealHardDiskDriver(const RealHardDiskDriver&) = delete;
    RealHardDiskDriver(RealHardDiskDriver&&) = delete;
    RealHardDiskDriver& operator = (const RealHardDiskDriver&) = delete;
    RealHardDiskDriver& operator = (RealHardDiskDriver&&) = delete;

    HardDiskDriver* getPartition(size_t i);

    /**
     * lba: size_t, 28bit LBA
     * buffer: char*
     * count: uint16_t, count of 256word
     **/
    int readOnPIOModel(size_t lba, char* buffer, uint16_t count);
    /**
     * lba: size_t, 28bit LBA
     * buffer: char*
     * count: uint16_t, count of 256word
     **/
    int writeOnPIOModel(size_t lba, const char* buffer, uint16_t count);

    /**
     * lba: size_t, 28bit LBA && 512 byte alignment
     * buffer: char*[]
     * count: uint16_t, count of PAGESIZE
     **/
    int readOnDMAMode(size_t lba, char* buffers[], uint16_t count);
    /**
     * lba: size_t, 28bit LBA && 512 byte alignment
     * buffer: char*[]
     * count: uint16_t, count of PAGESIZE
     * timeout: jiffies
     **/
    int readOnDMAMode(size_t lba, char* buffers[], uint16_t count, long timeout);

    /**
     * lba: size_t, 28bit LBA && 512 byte alignment
     * buffer: char*[]
     * count: uint16_t, count of PAGESIZE
     **/
    int writeOnDMAMode(size_t lba, char* buffers[], uint16_t count);
    /**
     * lba: size_t, 28bit LBA && 512 byte alignment
     * buffer: char*[]
     * count: uint16_t, count of PAGESIZE
     * timeout: jiffies
     **/
    int writeOnDMAMode(size_t lba, char* buffers[], uint16_t count, long timeout);

    void doIRQ();
};