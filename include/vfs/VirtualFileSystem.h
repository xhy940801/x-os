#pragma once

#include <vfs/DriverType.h>
#include <vfs/VfsType.h>

class VfsInode;

class VirtualFileSystem
{
    const char* _name;
    VfsType _type;
public:
    VirtualFileSystem(const char* name, VfsType type)
        : _name(name), _type(type)
    {
    }

    VirtualFileSystem(const VirtualFileSystem&) = delete; 

    const char* name() const
    {
        return _name;
    }

    VfsType type() const
    {
        return _type;
    }

public:
    virtual VfsInode* getRootInode(DriverType drivetype) {return nullptr;}
};

