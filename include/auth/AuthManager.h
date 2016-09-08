#pragma once

#include "type.h"

class AuthManager
{
    pid_t lastPid;

public:
    AuthManager() : lastPid(100) {}

    pid_t allocPid()
    {
        return lastPid++;
    }
};