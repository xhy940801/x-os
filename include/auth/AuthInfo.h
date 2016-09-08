#pragma once

#include "type.h"

class AuthInfo
{
    pid_t pid;

protected:
    AuthInfo() : pid(0) {}

public:
    AuthInfo(unsigned long options, const AuthInfo& auth);

    AuthInfo(const AuthInfo&) = delete;
    AuthInfo(AuthInfo&&) = delete;
    AuthInfo& operator = (const AuthInfo&) = delete;
    AuthInfo& operator = (AuthInfo&&) = delete;
};
