#pragma once

#include <cstddef>

#include "List.h"
#include "common.h"

class MultiProcInfo : public ListNode<MultiProcInfo, 0>
{
    union
    {
        size_t demand;
    };

    friend class Semaphore;
    friend class Mutex;
protected:
    MultiProcInfo()
    {
    }

public:
    MultiProcInfo(unsigned long options, const MultiProcInfo& info)
    {
        assert(lonely());
    }

    void interrupt();

    MultiProcInfo(const MultiProcInfo&) = delete;
    MultiProcInfo(MultiProcInfo&&) = delete;
    MultiProcInfo& operator = (const MultiProcInfo&) = delete;
    MultiProcInfo& operator = (MultiProcInfo&&) = delete;
};