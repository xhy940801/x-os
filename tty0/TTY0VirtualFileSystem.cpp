#include <tty0/TTY0VirtualFileSystem.h>

TTY0VirtualFileSystem::TTY0VirtualFileSystem(VfsInode* rootinode)
    : VirtualFileSystem("tty0", VfsType::TTY0), _rootinode(rootinode)
{
}

VfsInode* TTY0VirtualFileSystem::getRootInode(DriverType drivetype)
{
    return _rootinode->retain();
}
