#pragma once

#include "wait/WakeupRet.h"

#include "List.h"

class WaitInfo : public ListNode<WaitInfo, 0>
{
    long wakeupJiffies;
    WakeupRet ret;

protected:
    WaitInfo() = default;

public:
    void wait();
    void sleep(long jiffies);
    void wakeup(WakeupRet _ret);

    WaitInfo(unsigned long options, const WaitInfo& info) : ListNode<WaitInfo, 0>(), wakeupJiffies(info.wakeupJiffies), ret(info.ret) {}

    WaitInfo(const WaitInfo&) = delete;
    WaitInfo(WaitInfo&&) = delete;

    WaitInfo& operator = (const WaitInfo&) = delete;
    WaitInfo& operator = (WaitInfo&&) = delete;
};