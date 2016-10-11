#pragma once

#include "errno/ErrnoType.h"

#include "wait/WakeupRet.h"

class ErrnoInfo
{
    ErrnoType err;

protected:
    ErrnoInfo() : err(ErrnoType::ENONE) {}

public:
    ErrnoInfo(unsigned long options, const ErrnoInfo& info);

    ErrnoInfo(const ErrnoInfo&) = delete;
    ErrnoInfo(ErrnoInfo&&) = delete;
    ErrnoInfo& operator = (const ErrnoInfo&) = delete;
    ErrnoInfo& operator = (ErrnoInfo&&) = delete;

    void setErrno(ErrnoType e)
    {
        err = e;
    }

    void setErrno(WakeupRet ret);

    ErrnoType errno() const
    {
        return err;
    }
};