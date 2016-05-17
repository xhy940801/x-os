#pragma once

#include <vfs.h>

#include <tty0/TTY0VfsInode.h>

class TTY0VirtualFileSystem : public VirtualFileSystem
{
    VfsInode* _rootinode;
public:
    TTY0VirtualFileSystem(VfsInode* rootinode);
    virtual VfsInode* getRootInode(DriverType drivetype);
};
