#include <new>

#include "auth/AuthInfo.h"
#include "auth/AuthManager.h"

AuthManager authManager;

void initAuthModule()
{
    new (&authManager) AuthManager();
}