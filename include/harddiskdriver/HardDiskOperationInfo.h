#pragma once

#include <cstddef>
#include <cstdint>

#include "List.h"

#include "mem.h"

#include "blockdriver/commondefs.h"

class TaskInfo;

class HardDiskOperationInfo : public ListNode<HardDiskOperationInfo, 0>
{
    enum class OperationType : uint16_t
    {
        READ, WRITE
    };

private:
    uint16_t length;
    OperationType operationType;
    size_t startLBA;
    TaskInfo* taskInfo;
    char** p;

public:
    void initForRead(size_t start, uint16_t len, char* buf[], TaskInfo* t)
    {
        startLBA = start;
        length = len;
        operationType = OperationType::READ;
        p = buf;
        taskInfo = t;
    }

    void initForWrite(size_t start, uint16_t len, char* buf[], TaskInfo* t)
    {
        startLBA = start;
        length = len;
        operationType = OperationType::WRITE;
        p = buf;
        taskInfo = t;
    }

    void setResult(size_t end, uint16_t len)
    {
        startLBA = end;
        length = len;
    }

    void setSuccessResult()
    {
        startLBA = length * (mem::PAGESIZE / 512);
        length = 0;
    }

    size_t lba() const
    {
        return startLBA;
    }

    uint16_t count() const
    {
        return length;
    }

    OperationType type() const
    {
        assert(operationType == OperationType::READ || operationType == OperationType::WRITE);
        return operationType;
    }

    bool readType() const
    {
        return type() == OperationType::READ;
    }

    char** buffers()
    {
        return p;
    }

    const TaskInfo* task() const
    {
        return taskInfo;
    }

    TaskInfo* task()
    {
        return taskInfo;
    }

    uint32_t physicalAddr(size_t i)
    {
        assert(i < count());
        assert(operationType == OperationType::READ || operationType == OperationType::WRITE);
        return memoryManager.getPhysicalAddr(p[i]);
    }
};