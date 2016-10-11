#pragma once

#include "List.h"

#include "wait/WakeupRet.h"

class MultiProcInfo;

class Mutex
{
    List<MultiProcInfo, 0> list;

public:
    Mutex() {}
    ~Mutex() {}

    Mutex(const Mutex&) = delete;
    Mutex(Mutex&&) = delete;
    Mutex& operator = (const Mutex&) = delete;
    Mutex& operator = (Mutex&&) = delete;

    bool tryLock();

    WakeupRet lock();

    WakeupRet lock(long timeout);

    void unlock();
};