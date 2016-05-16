#pragma once

#include <cstdint>
#include <cstddef>

#include <common.h>

#include <list.h>

#include <vfs/DriverType.h>

class FdInfo;

class VfsInode : public ListNode<VfsInode, 0>
{
    uint32_t _fsystype;
    DriverType _drivertype;
    VfsInode* _parent;
    List<VfsInode, 0> _children;
    size_t _opencount;
public:
    VfsInode(uint32_t fsystype, DriverType drivertype, VfsInode* parent)
        : ListNode<VfsInode, 0>(parent->_children),
        _fsystype(fsystype), _drivertype(drivertype), _parent(parent), _opencount(0)
    {
    }

    VfsInode(const VfsInode&) = delete;

    ~VfsInode()
    {
    }

    VfsInode* retain()
    {
        ++_opencount;
        return this;
    }

    void release()
    {
        --_opencount;
        if(_opencount == 0)
        {
            ListNode<VfsInode, 0>::removeSelf();
            _parent->release();
            this->close();
        }
    }

    uint32_t fsystype() const
    {
        return _fsystype;
    }

    DriverType drivertype() const
    {
        return _drivertype;
    }

public:
    virtual VfsInode* openChild(const char* subpath, size_t len);
    virtual ssize_t read(uint8_t* buf, size_t len, FdInfo* info);
    virtual ssize_t write(const uint8_t* buf, size_t len, FdInfo* info);
    virtual void flush();
    virtual void close() = 0;//to do something and delete this
};
