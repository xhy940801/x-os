#pragma once

#include "vfs/VfsInode.h"

class VfsManager
{
    VfsInode rootInode;
public:
    VfsInode* root()
    {
        return rootInode.retain();
    }
};

extern VfsManager vfsManager;