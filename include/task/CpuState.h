#pragma once

#include <cstdint>

#include "common.h"

class CpuState
{
    uintptr_t catalogTableP;
    char fpuState[108];

    CpuState(uintptr_t _catalogTableP) : catalogTableP(_catalogTableP)
    {
        memset(fpuState, 0, sizeof(fpuState));
    }

    friend class TaskManager;
    friend class TaskInfo;
public:
    CpuState(uintptr_t _catalogTableP, unsigned long options, const CpuState& cpuState)
        : catalogTableP(_catalogTableP)
    {
        memcpy(fpuState, cpuState.fpuState, sizeof(fpuState));
    }

    CpuState() = delete;
    CpuState(const CpuState&) = delete;
    CpuState(CpuState&&) = delete;
    CpuState& operator = (const CpuState&) = delete;
    CpuState& operator = (CpuState&&) = delete;
};