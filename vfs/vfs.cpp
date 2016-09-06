#include <new>

#include "vfs/VfsManager.h"

VfsManager vfsManager;

void initVfsModule()
{
    new (&vfsManager) VfsManager();
}
