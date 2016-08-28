#include "vfs/VfsInode.h"

VfsInode* VfsInode::openChild(const char* subpath, size_t len)
{
    return nullptr;
}

ssize_t VfsInode::read(uint8_t* buf, size_t len, FdInfo* info)
{
    return -1;
}

ssize_t VfsInode::write(const uint8_t* buf, size_t len, FdInfo* info)
{
    return -1;
}

void VfsInode::flush()
{
}

void VfsInode::close()
{
}
