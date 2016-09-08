#include "wait/WaitManager.h"

#include <new>

WaitManager waitManager;

void initWaitModule()
{
    new (&waitManager) WaitManager();
}