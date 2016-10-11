#pragma once

#include "List.h"

#include "wait/WakeupRet.h"

class MultiProcInfo;

class Cond
{
    List<MultiProcInfo, 0> list;

public:
    Cond() {}
    ~Cond() {}

    Cond(const Cond&) = delete;
    Cond(Cond&&) = delete;
    Cond& operator = (const Cond&) = delete;
    Cond& operator = (Cond&&) = delete;

    WakeupRet wait();

    WakeupRet wait(long timeout);

    void signal();

    void broadcast();
};