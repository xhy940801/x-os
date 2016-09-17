#pragma once

#include <type_traits>

#include "common.h"

template <typename T, int N = 0>
class List;

template <typename T, int N = 0>
class ListNode
{
    friend class List<T, N>;
protected:
    ListNode* _next;
    ListNode* _prev;

    ListNode()
        : _next(this), _prev(this)
    {
        static_assert(std::is_base_of<ListNode<T, N>, T>::value, "class ListNode<T, N> must base of T");
    }

    ListNode(ListNode* node)
    {
        node->insertBefore(*this);
    }

    inline ListNode(List<T, N>& list);
public:

    void insertAfter(ListNode& next)
    {
        next._next = this->_next;
        next._prev = this;

        this->_next = &next;
        next._next->_prev = &next;
    }

    void insertBefore(ListNode& prev)
    {
        prev._prev = this->_prev;
        prev._next = this;
        this->_prev = &prev;
        prev._prev->_next = &prev;
    }

    void removeSelf()
    {
        assert(_prev != this && _next != this);
        _prev->_next = _next;
        _next->_prev = _prev;
    }

    T* next()
    {
        return static_cast<T*>(_next);
    }

    T* prev()
    {
        return static_cast<T*>(_prev);
    }
};

template <typename T, int N>
class List
{
    friend class ListNode<T, N>;
    ListNode<T, N> head;
public:
    void pushFront(ListNode<T, N>& node)
    {
        head.insertAfter(node);
    }

    void pushBack(ListNode<T, N>& node)
    {
        head.insertBefore(node);
    }

    bool empty()
    {
        bool isEmpty = head._prev == &head;
        if (isEmpty)
            assert(head._prev == &head && head._next == &head);
        return isEmpty;
    }

    T* begin()
    {
        return head.next();
    }

    T* rbegin()
    {
        return head.prev();
    }

    T* end()
    {
        return &head;
    }

    T* rend()
    {
        return &head;
    }
};

template <typename T, int N>
inline ListNode<T, N>::ListNode(List<T, N>& list)
{
    static_assert(std::is_base_of<ListNode<T, N>, T>::value, "class ListNode<T, N> must base of T");
    list.pushBack(*static_cast<T*>(this));
}
