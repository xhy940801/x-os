#pragma once

enum class ErrnoType
{
    ENONE       = 0,
    EPERM       = 1,
    EINTR       = 4,
    ENXIO       = 6,
    EAGAIN      = 11,
    ENOMEM      = 12,
    ENODEV      = 19,
    ENOSPC      = 28,
    ETIMEDOUT   = 110
};