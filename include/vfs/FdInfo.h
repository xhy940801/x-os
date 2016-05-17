#pragma once

#include <vfs/VfsInode.h>

class FdInfo
{
    VfsInode* _inode;
    uint32_t _flags;
    uint32_t _pos;
public:
    FdInfo(VfsInode* inode, uint32_t flags, uint32_t pos)
        : _inode(inode), _flags(flags), _pos(pos)
    {
    }

    FdInfo(const FdInfo& info)
        : _inode(info._inode.retain()), _flags(info._flags), _pos(info._pos)
    {
    }

    FdInfo(FdInfo&& info)
        : _inode(info._inode), _flags(info._flags), _pos(info._pos)
    {
        info._inode = nullptr;
    }

    ~FdInfo()
    {
        if(_inode)
            _inode->release();
    }

    const VfsInode* inode() const
    {
        return _inode;
    }

    VfsInode* inode()
    {
        return _inode;
    }

    uint32_t flags() const
    {
        return _flags;
    }

    uint32_t pos() const
    {
        return pos;
    }
}
