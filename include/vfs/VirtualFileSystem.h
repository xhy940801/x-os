#pragma once

#include <vfs/DriverType.h>

class VfsInode;

class VirtualFileSystem
{
    const char* _name;
public:
    VirtualFileSystem(const char* name)
        : _name(name)
    {
    }

    VirtualFileSystem(const VirtualFileSystem&) = delete; 

    const char* name()
    {
        return _name;
    }

public:
    virtual VfsInode* getRootInode(DriverType drivetype) = 0;
};
