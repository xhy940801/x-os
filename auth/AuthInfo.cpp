#include "auth/AuthInfo.h"

#include "auth.h"

AuthInfo::AuthInfo(unsigned long options, const AuthInfo& auth)
    : pid(authManager.allocPid())
{
}