#include "task.h"

#include <new>

TaskManager taskManager;

void initTaskModule()
{
    new (&taskManager) TaskManager();
}