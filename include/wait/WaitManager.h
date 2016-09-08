#pragma once

#include "wait/TimeWheel.h"
#include "wait/WaitInfo.h"
#include "wait/commondefs.h"

class WaitManager
{
    struct JiffiesSegmentation
    {
        union
        {
            long jiffies;
            uint8_t segmentation[wait::WHEEL_SIZE];
        };
        
        JiffiesSegmentation(long _jiffies) : jiffies(_jiffies) {}
    };

private:
    TimeWheel sleepWheels[wait::WHEEL_SIZE];
    List<WaitInfo, 0> waitList;

    void wait(WaitInfo* info)
    {
        waitList.pushBack(*info);
    }

    void sleep(WaitInfo* info, long jiffies);

    friend class WaitInfo;

    void moveWheel(size_t level, JiffiesSegmentation curJiffies);

public:
    void wakeup();

    WaitManager() {}
};