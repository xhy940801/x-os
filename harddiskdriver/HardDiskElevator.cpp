#include "harddiskdriver/HardDiskElevator.h"

#include "harddiskdriver/HardDiskOperationInfo.h"

#include "common.h"

HardDiskElevator::HardDiskElevator()
    : turn(list.begin()), status(Status::DOWN)
{
}

void HardDiskElevator::push(HardDiskOperationInfo& info)
{
    if (list.empty())
    {
        list.pushBack(info);
        turn = --list.end();
        return;
    }

    assert(good());

    if ((status == Status::DOWN && info.lba() < list.begin()->lba()) ||
        (status == Status::UP && info.lba() > list.begin()->lba()))
    {
        auto cur = list.begin();
        auto end = turn;
        ++end;
        ++cur;
        for (; cur != end; ++cur)
        {
            if (status == Status::DOWN && info.lba() > cur->lba())
                break;
            if (status == Status::UP && info.lba() < cur->lba())
                break;
        }
        cur->insertBefore(info);
        if (cur == end)
            turn = (--cur);
    }
    else
    {
        auto cur = turn;
        ++cur;
        for (; cur != list.end(); ++cur)
        {
            if (status == Status::DOWN && info.lba() < cur->lba())
                break;
            if (status == Status::UP && info.lba() > cur->lba())
                break;
        }
        cur->insertBefore(info);
    }

    assert(good());
}

void HardDiskElevator::pop()
{
    assert(!list.empty());
    assert(good());

    HardDiskOperationInfo& info = *list.begin();

    bool change = list.begin() == turn;
    info.removeSelf();
    if (change)
    {
        turn = --list.end();
        status = status != Status::UP ? Status::UP : Status::DOWN;
    }

    assert(good());
}

HardDiskOperationInfo& HardDiskElevator::top()
{
    return *list.begin();
}

const HardDiskOperationInfo& HardDiskElevator::top() const
{
    return *list.begin();
}

bool HardDiskElevator::good() const
{
    if (status != Status::UP && status != Status::DOWN)
        return false;
    if (list.empty())
    {
        if (turn != list.begin())
            return false;
        return true;
    }

    if (list.begin() != turn)
    {
        auto prev = list.begin();
        auto cur = list.begin();
        ++cur;
        auto end = turn;
        ++end;
        for (; cur != end; ++cur, ++prev)
        {
            if (status == Status::UP && cur->lba() < prev->lba())
                return false;
            if (status == Status::DOWN && cur->lba() > prev->lba())
                return false;
        }
    }
    if ((--list.end()) != turn && (--(--list.end())) != turn)
    {
        auto prev = turn;
        ++prev;
        auto cur = prev;
        ++cur;
        for (; cur != list.end(); ++cur, ++prev)
        {
            if (status == Status::UP && cur->lba() > prev->lba())
                return false;
            if (status == Status::DOWN && cur->lba() < prev->lba())
                return false;
        }
    }
    auto nxt = turn;
    ++nxt;
    if (nxt != list.end())
    {
        if (status == Status::UP && list.begin()->lba() < nxt->lba())
            return false;
        if (status == Status::DOWN && list.begin()->lba() > nxt->lba())
            return false;
    }
    return true;
}