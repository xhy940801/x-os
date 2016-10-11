#include "harddiskdriver/RealHardDiskDriver.h"

#include "common.h"

#include "task.h"
#include "pci.h"
#include "mem.h"
#include "sched.h"

static inline size_t getATALBA(int basePort)
{
    size_t lba = ((size_t) _inb(basePort + harddiskdriver::PORT_DRIVERSELECT)) & 0x0f;
    lba <<= 8;
    lba |= ((size_t) _inb(basePort + harddiskdriver::PORT_LBAHI)) & 0xff;
    lba <<= 8;
    lba |= ((size_t) _inb(basePort + harddiskdriver::PORT_LBAMID)) & 0xff;
    lba <<= 8;
    lba |= ((size_t) _inb(basePort + harddiskdriver::PORT_LBALO)) & 0xff;
    return lba;
}

void RealHardDiskDriver::read256word(uint16_t* data)
{
    __asm__ (
        "rep insw"
        ::"d"(basePort + harddiskdriver::PORT_DATA),"c"(256),"D"(data)
        :"memory"
    );
}
void RealHardDiskDriver::write256word(const uint16_t* data)
{
    for(size_t i = 0; i < 256; ++i)
        __asm__ (
            "outw %%ax, %%dx"
            ::"d"(basePort + harddiskdriver::PORT_DATA),"a"(data[i])
        );
}

RealHardDiskDriver::RealHardDiskDriver(int _basePort, int _slaveBit)
    : basePort(_basePort), slaveBit(_slaveBit), sem(harddiskdriver::OPERATION_CAPACITY)
{
    assert(slaveBit == 0 || slaveBit == 0x10);
    for (auto& op : operations)
        freeList.pushBack(op);

    //Load HD params
    //Set data to port
    _outb(basePort + harddiskdriver::PORT_DRIVERSELECT, 0xa0 | slaveBit);
    _outb(basePort + harddiskdriver::PORT_SELECTORCOUNT, 0);
    _outb(basePort + harddiskdriver::PORT_LBALO, 0);
    _outb(basePort + harddiskdriver::PORT_LBAMID, 0);
    _outb(basePort + harddiskdriver::PORT_LBAHI, 0);
    _outb(basePort + harddiskdriver::PORT_CMDSTATUS, 0xec);

    while (1)
    {
        int status = _inb(basePort + harddiskdriver::PORT_CMDSTATUS);
        //Drive does not exist
        if (status == 0)
            abort();
        if((status & 0x80) == 0)
            break;
    }
    //The drive is not ATA. abort
    if (_inb(basePort + harddiskdriver::PORT_LBAMID) != 0)
        abort();
    if (_inb(basePort + harddiskdriver::PORT_LBAHI) != 0)
        abort();
    while (1)
    {
        int status = _inb(basePort + harddiskdriver::PORT_CMDSTATUS);
        //Error
        if (status & 0x01)
            abort();
        //Ready to read
        if (status & 0x08)
            break;
    }
    //Read hard disk info
    uint16_t datas[256];
    read256word(datas);
    len = *((uint32_t*) (datas + 60));
    if (len == 0)
        abort();
    resetPartitionTable();
    initPartitionTable();
    initBMR();
}

int RealHardDiskDriver::readOnPIOModel(size_t lba, char* buffer, uint16_t count)
{
    //Just support 28 bit lba
    assert((lba & 0xf0000000) == 0);
    auto locker = taskManager.curtask()->getLocker();
    _outb(basePort + harddiskdriver::PORT_DRIVERSELECT, 0xe0 | slaveBit | (lba >> 24));
    _outb(basePort + harddiskdriver::PORT_SELECTORCOUNT, count);
    _outb(basePort + harddiskdriver::PORT_LBALO, lba & 0xff);
    _outb(basePort + harddiskdriver::PORT_LBAMID, (lba >> 8) & 0xff);
    _outb(basePort + harddiskdriver::PORT_LBAHI, (lba >> 16) & 0xff);
    _outb(basePort + harddiskdriver::PORT_CMDSTATUS, 0x20);
    //400ns delay
    int status;
    _inb(basePort + harddiskdriver::PORT_CMDSTATUS);
    _inb(basePort + harddiskdriver::PORT_CMDSTATUS);
    _inb(basePort + harddiskdriver::PORT_CMDSTATUS);
    _inb(basePort + harddiskdriver::PORT_CMDSTATUS);
    for (size_t i = 0; i < count; ++i)
    {
        while(1)
        {
            status = _inb(basePort + harddiskdriver::PORT_CMDSTATUS);
            if (status & 0x21)
            {
                //TODO RESET Hard Disk Drive
                return i == 0 ? -1 : i;
            }
            if ((status & 0x80) == 0)
                break;
        }
        while ((status & 0x08) == 0)
            status = _inb(basePort + harddiskdriver::PORT_CMDSTATUS);
        read256word(reinterpret_cast<uint16_t*>(buffer));
        buffer += 512;
    }
    return count;
}

int RealHardDiskDriver::writeOnPIOModel(size_t lba, const char* buffer, uint16_t count)
{
    //Just support 28 bit lba
    assert((lba & 0xf0000000) == 0);
    auto locker = taskManager.curtask()->getLocker();
    _outb(basePort + harddiskdriver::PORT_DRIVERSELECT, 0xe0 | slaveBit | (lba >> 24));
    _outb(basePort + harddiskdriver::PORT_SELECTORCOUNT, count);
    _outb(basePort + harddiskdriver::PORT_LBALO, lba & 0xff);
    _outb(basePort + harddiskdriver::PORT_LBAMID, (lba >> 8) & 0xff);
    _outb(basePort + harddiskdriver::PORT_LBAHI, (lba >> 16) & 0xff);
    _outb(basePort + harddiskdriver::PORT_CMDSTATUS, 0x30);
    //400ns delay
    int status;
    _inb(basePort + harddiskdriver::PORT_CMDSTATUS);
    _inb(basePort + harddiskdriver::PORT_CMDSTATUS);
    _inb(basePort + harddiskdriver::PORT_CMDSTATUS);
    _inb(basePort + harddiskdriver::PORT_CMDSTATUS);
    for (size_t i = 0; i < count; ++i)
    {
        while(1)
        {
            status = _inb(basePort + harddiskdriver::PORT_CMDSTATUS);
            if (status & 0x21)
            {
                //TODO RESET Hard Disk Drive
                return i == 0 ? -1 : i;
            }
            if ((status & 0x80) == 0)
                break;
        }
        while ((status & 0x08) == 0)
            status = _inb(basePort + harddiskdriver::PORT_CMDSTATUS);
        write256word(reinterpret_cast<const uint16_t*>(buffer));
        buffer += 512;
    }
    return count;
}

void RealHardDiskDriver::resetPartitionTable()
{
    struct LBA48PartitionTable
    {
        uint8_t flags;
        uint8_t signature1;
        uint16_t lbahi;
        uint8_t systemid;
        uint8_t signature2;
        uint16_t lenhi;
        uint32_t lbalo;
        uint32_t lenlo;
    };

    char buf[512];
    int ret = readOnPIOModel(0, buf, 1);
    if (ret != 1)
        abort();
    LBA48PartitionTable* table1 = reinterpret_cast<LBA48PartitionTable*>(buf + 446);
    table1->flags = 0x81;
    table1->signature1 = 0x14;
    table1->signature2 = 0xeb;
    table1->lbahi = 0;
    table1->lenhi = 0;
    table1->lbalo = 0;
    table1->lenlo = 2 * 1024;

    LBA48PartitionTable* table2 = reinterpret_cast<LBA48PartitionTable*>(buf + 446 + 16);
    table2->flags = 0x01;
    table2->signature1 = 0x14;
    table2->signature2 = 0xeb;
    table2->lbahi = 0;
    table2->lenhi = 0;
    table2->lbalo = 2 * 1024;
    table2->lenlo = 20 * 1024 - 2 * 1024;
    ret = writeOnPIOModel(0, buf, 1);
    if (ret != 1)
        abort();
}

void RealHardDiskDriver::initPartitionTable()
{
    struct LBA48PartitionTable
    {
        uint8_t flags;
        uint8_t signature1;
        uint16_t lbahi;
        uint8_t systemid;
        uint8_t signature2;
        uint16_t lenhi;
        uint32_t lbalo;
        uint32_t lenlo;
    };

    char buf[512];
    int ret = readOnPIOModel(0, buf, 1);
    if (ret != 1)
        abort();
    for (size_t i = 0; i < 4; ++i)
    {
        LBA48PartitionTable* table = reinterpret_cast<LBA48PartitionTable*>(buf + 446 + i * 16);
        if(table->flags != 0x01 && table->flags != 0x81)
            continue;
        if(table->signature1 != 0x14 || table->signature2 != 0xeb)
            continue;
        if(table->lbahi != 0)
            continue;
        if(table->lenhi != 0)
            continue;
        size_t lba = table->lbalo;
        size_t length = table->lenlo;
        partitions[i].init(lba, length, this);
    }
}

void RealHardDiskDriver::initBMR()
{
    PCIInfo info;
    if (!enumeratingATAController(&info))
        abort();
    assert(info.bar4 & 0x01);
    bmr.basePort = info.bar4 & 0xfffffffc;
    bmr.prdt = static_cast<PRDTInfo*>(memoryManager.getOnePersistePage(bmr.prdtPhysicalAddress));
}

bool RealHardDiskDriver::enumeratingATAController(PCIInfo* info)
{
    for (uint8_t bus = 0; bus < 256; ++bus)
    {
        for (uint8_t slot = 0; slot < 32; ++slot)
            for (uint8_t function = 0; function < 8; ++function)
            {
                loadPCIConfigurationSpace(info, bus, slot, function, 0);
                if (info->vendorID != 0xffff && info->classCode == 0x01 && info->subclass == 0x01)
                    return true;
            }
        if (bus == 255)
            break;
    }
    return false;
}

int RealHardDiskDriver::readOnDMAModeImmediately(size_t lba, char* buffers[], uint16_t count)
{
    TaskInfo* task = taskManager.curtask();
    auto locker = task->getLocker();
    assert(!freeList.empty());
    auto it = freeList.begin();
    it->initForRead(lba, count, buffers, task);
    it->removeSelf();
    bool needStart = elevator.empty();
    elevator.push(*it);

    if (needStart)
        processOneOperation();
    else
        task->wait();
    scheduleManager.schedule();

    WakeupRet ret = task->wakeupRet();
    if (ret != WakeupRet::NORMAL)
    {
        task->setErrno(ret);
        return -1;
    }
    
    assert(count >= it->count());
    int rs = count - it->count();
    assert(rs >= 0);
    if (rs == 0)
    {
        //TODO set errno
        return -1;
    }
    return rs;
}

void fuck()
{

}

int RealHardDiskDriver::writeOnDMAModeImmediately(size_t lba, char* buffers[], uint16_t count)
{
    TaskInfo* task = taskManager.curtask();
    auto locker = task->getLocker();
    assert(!freeList.empty());
    auto it = freeList.begin();
    it->initForWrite(lba, count, buffers, task);
    it->removeSelf();
    bool needStart = elevator.empty();
    elevator.push(*it);

    WakeupRet ret = task->wakeupRet();
    if (needStart)
        processOneOperation();
    else
        task->wait();

    scheduleManager.schedule();
    ret = task->wakeupRet();
    if (ret != WakeupRet::NORMAL)
    {
        task->setErrno(ret);
        return -1;
    }

    assert(count >= it->count());
    int rs = count - it->count();
    assert(rs >= 0);
    if (rs == 0)
    {
        //TODO set errno
        return -1;
    }
    return rs;
}

void RealHardDiskDriver::processOneOperation()
{
    HardDiskOperationInfo& info = elevator.top();
    for (size_t i = 0; i < info.count(); ++i)
    {
        bmr.prdt[i].physicalAddress = info.physicalAddr(i);
        bmr.prdt[i].byteCount = mem::PAGESIZE;
        bmr.prdt[i].eot = 0x0000;
    }
    bmr.prdt[info.count() - 1].eot = 0x8000;

    _outd(bmr.basePort + harddiskdriver::PORT_BMI_M_PRDTADDR, bmr.prdtPhysicalAddress);
    _outd(bmr.basePort + harddiskdriver::PORT_BMI_M_STATUS, 0x06);

    _outb(basePort + harddiskdriver::PORT_DRIVERSELECT, 0xe0 | slaveBit | ((info.lba() >> 24) & 0x0f));
    _outb(basePort + harddiskdriver::PORT_SELECTORCOUNT, info.count() << 3);
    _outb(basePort + harddiskdriver::PORT_LBALO, info.lba() & 0xff);
    _outb(basePort + harddiskdriver::PORT_LBAMID, ((info.lba() >> 8) & 0xff));
    _outb(basePort + harddiskdriver::PORT_LBAHI, ((info.lba() >> 16) & 0xff));

    if (info.readType())
    {
        _outb(basePort + harddiskdriver::PORT_CMDSTATUS, 0xc8);
        _outb(bmr.basePort + harddiskdriver::PORT_BMI_M_COMMAND, 0x09);
    }
    else
    {
        _outb(basePort + harddiskdriver::PORT_CMDSTATUS, 0xca);
        _outb(bmr.basePort + harddiskdriver::PORT_BMI_M_COMMAND, 0x01);
    }

    info.task()->uninterruptableWait();
}

int RealHardDiskDriver::readOnDMAMode(size_t lba, char* buffers[], uint16_t count)
{
    WakeupRet ret = sem.down(1);
    if (ret != WakeupRet::NORMAL)
    {
        taskManager.curtask()->setErrno(ret);
        return -1;
    }
    return readOnDMAModeImmediately(lba, buffers, count);
}

int RealHardDiskDriver::readOnDMAMode(size_t lba, char* buffers[], uint16_t count, long timeout)
{
    WakeupRet ret = sem.down(1, timeout);
    if (ret != WakeupRet::NORMAL)
    {
        taskManager.curtask()->setErrno(ret);
        return -1;
    }
    return readOnDMAModeImmediately(lba, buffers, count);
}

int RealHardDiskDriver::writeOnDMAMode(size_t lba, char* buffers[], uint16_t count)
{
    WakeupRet ret = sem.down(1);
    if (ret != WakeupRet::NORMAL)
    {
        taskManager.curtask()->setErrno(ret);
        return -1;
    }
    return writeOnDMAModeImmediately(lba, buffers, count);
}

int RealHardDiskDriver::writeOnDMAMode(size_t lba, char* buffers[], uint16_t count, long timeout)
{
    WakeupRet ret = sem.down(1, timeout);
    if (ret != WakeupRet::NORMAL)
    {
        taskManager.curtask()->setErrno(ret);
        return -1;
    }
    return writeOnDMAModeImmediately(lba, buffers, count);
}

void RealHardDiskDriver::doIRQ()
{
    printk("hd doIRQ\n");
    auto locker = taskManager.curtask()->getLocker();

    int dmaStatus = _inb(bmr.basePort + harddiskdriver::PORT_BMI_M_STATUS);
    int ataStatus = _inb(basePort + harddiskdriver::PORT_CMDSTATUS);

    // TODO
    int ataError = _inb(basePort + harddiskdriver::PORT_ERROR);
    assert(ataError || !ataError);

    HardDiskOperationInfo& info = elevator.top();
    if (dmaStatus & 0x02 || ataStatus & 0x01)
    {
        size_t lba = getATALBA(basePort);
        assert(lba >= info.lba());
        uint16_t count = (lba - info.lba()) / mem::PAGESIZE;
        info.setResult(lba, info.count() - count);
    }

    elevator.pop();
    info.task()->wakeup(WakeupRet::NORMAL);
    freeList.pushFront(info);
    if (!elevator.empty())
        processOneOperation();
}

HardDiskDriver* RealHardDiskDriver::getPartition(size_t i)
{
    assert(i < sizeof(partitions) / sizeof(partitions[0]));
    return partitions + i;
}