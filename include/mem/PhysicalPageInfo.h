#pragma once

#include <cstdint>

#include <mem/commondefs.h>

class TaskInfo;

class PhysicalMemoryManager;

class PhysicalPageInfo
{
public:
    enum class Type : uint8_t
    {
        KERNEL,
        NORMAL
    };
private:
    friend class PhysicalMemoryManager;
    uint8_t _active;
    Type _type;
    uint16_t _share;
    union
    {
        PhysicalPageInfo* _next;
        TaskInfo* _task;
    };
public:
    PhysicalPageInfo(Type type, PhysicalPageInfo* next)
        : _active(mem::USHRTMAX), _type(type), _share(0), _next(next)
    {
    }

    PhysicalPageInfo(Type type, TaskInfo* task)
        : _active(mem::USHRTMAX), _type(type), _share(1), _task(task)
    {
    }

    uint8_t active() const
    {
        return _active;
    }

    Type type() const
    {
        return _type;
    }

    uint32_t share() const
    {
        return _share;
    }

    PhysicalPageInfo* next()
    {
        return _next;
    }

    const PhysicalPageInfo* next() const
    {
        return _next;
    }

    TaskInfo* task()
    {
        return _task;
    }

    const TaskInfo* task() const
    {
        return _task;
    }
};

static_assert(sizeof(PhysicalPageInfo) == 8, "sizeof(PhysicalPageInfo) must equal to 8");