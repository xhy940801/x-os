#pragma once

#include "type.h"

class AuthInfo
{
    pid_t pid;

    AuthInfo(pid_t _pid) : pid(_pid) {}

    friend class TaskManager;
    friend class TaskInfo;
public:
    AuthInfo(pid_t _pid, unsigned long options, const AuthInfo& auth) : pid(_pid) {}

    AuthInfo() = delete;
    AuthInfo(const AuthInfo&) = delete;
    AuthInfo(AuthInfo&&) = delete;
    AuthInfo& operator = (const AuthInfo&) = delete;
    AuthInfo& operator = (AuthInfo&&) = delete;
};
