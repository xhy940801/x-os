#include "errno/ErrnoInfo.h"

#include "common.h"

ErrnoInfo::ErrnoInfo(unsigned long options, const ErrnoInfo& info)
    : err(info.err)
{
}

void ErrnoInfo::setErrno(WakeupRet ret)
{
    switch (ret)
    {
    case WakeupRet::TIMEOUT:
        err = ErrnoType::ETIMEDOUT;
        break;
    default:
        abort();
    }
}