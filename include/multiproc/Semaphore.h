#pragma once

#include <cstdint>

#include "List.h"

#include "wait/WakeupRet.h"

class MultiProcInfo;

class Semaphore
{
    List<MultiProcInfo, 0> list;
    size_t resource;

public:
    Semaphore() : resource(0) {}
    Semaphore(size_t r) : resource(r) {}
    ~Semaphore() {}

    Semaphore(const Semaphore&) = delete;
    Semaphore(Semaphore&&) = delete;
    Semaphore& operator = (const Semaphore&) = delete;
    Semaphore& operator = (Semaphore&&) = delete;

    bool tryDown(size_t demand);

    WakeupRet down(size_t demand);

    WakeupRet down(size_t demand, long timeout);

    void up(size_t num);

    size_t resourceCount()
    {
        return resource;
    }
};