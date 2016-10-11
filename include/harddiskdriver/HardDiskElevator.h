#pragma once

#include "List.h"

class HardDiskOperationInfo;

class HardDiskElevator
{
    enum class Status
    {
        UP,
        DOWN
    };

private:
    List<HardDiskOperationInfo, 0>::Iterator turn;
    List<HardDiskOperationInfo, 0> list;
    Status status;

public:
    HardDiskElevator();

    void push(HardDiskOperationInfo& info);
    void pop();
    HardDiskOperationInfo& top();
    const HardDiskOperationInfo& top() const;

    bool good() const;
    bool empty() const
    {
        return list.empty();
    }
};