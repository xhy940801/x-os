#include <new>

#include <vfs.h>

#include <tty0/TTY0VfsInode.h>
#include <tty0/TTY0VirtualFileSystem.h>

uint8_t tty0inode[sizeof(TTY0VfsInode)];
uint8_t tty0vfs[sizeof(TTY0VirtualFileSystem)];

void initTTY0Module()
{
    TTY0VfsInode* vfsinode = reinterpret_cast<TTY0VfsInode*>(tty0inode);
    TTY0VirtualFileSystem* vfs = reinterpret_cast<TTY0VirtualFileSystem*>(tty0vfs);
    new (vfsinode) TTY0VfsInode(
        VfsType::TTY0,
        DriverType(0, MainDriverType::TTY0),
        nullptr,
        reinterpret_cast<TTY0VfsInode::TTY0Character*>(0xfffb8000)
    );
    new (vfs) TTY0VirtualFileSystem(vfsinode);
}
