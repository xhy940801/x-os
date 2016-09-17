#pragma once

#include "wait/WakeupRet.h"

#include "List.h"

class WaitInfo
{
    long wakeupJiffies;
    WakeupRet ret;

    friend class WaitManager;

protected:
    WaitInfo() = default;

public:
    void wait();
    void sleep(long jiffies);
    void wakeup(WakeupRet _ret);

    WaitInfo(unsigned long options, const WaitInfo& info) : wakeupJiffies(info.wakeupJiffies), ret(info.ret) {}

    WaitInfo(const WaitInfo&) = delete;
    WaitInfo(WaitInfo&&) = delete;

    WaitInfo& operator = (const WaitInfo&) = delete;
    WaitInfo& operator = (WaitInfo&&) = delete;
};