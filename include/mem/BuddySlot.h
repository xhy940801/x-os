#pragma once

#include <cstddef>
#include <cstdint>

#include "list.h"
#include "bitmap.h"
#include "common.h"

class BuddySlot
{
    class Node : public ListNode<Node, 0>
    {
    };

private:
    Node* _nodes;
    Bitmap<uint32_t> _map;
    List<Node, 0> _list;

public:
    void init(size_t listSize);
    //return -1 if getAddress fail
    int getNode()
    {
        if(_list.empty())
            return -1;
        Node* node = _list.begin();
        node->removeSelf();
        int pos = node - _nodes;
        assert(pos >= 0);
        _map.flip(pos / 2);
        return pos;
    }
    void freeNode(size_t pos)
    {
        _list.pushFront(_nodes[pos]);
        _map.flip(pos / 2);
    }
    //return false if it need merge
    bool freeNodeAndMerge(size_t pos)
    {
        Node* node = _nodes + pos;
        pos /= 2;
        _map.flip(pos);
        bool rs = _map.value(pos);
        if(rs)
            _list.pushFront(*node);
        return rs;
    }

    bool nodeState(size_t pos)
    {
        return _map.value(pos / 2);
    }
};
