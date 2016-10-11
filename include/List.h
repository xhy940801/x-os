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

        _prev = this;
        _next = this;
    }

    bool lonely() const
    {
        bool isLonely = _prev == this;
        if (isLonely)
            assert(_prev == this && _next == this);
        return isLonely;
    }

    T* next()
    {
        return static_cast<T*>(_next);
    }

    const T* next() const
    {
        return static_cast<const T*>(_next);
    }

    T* prev()
    {
        return static_cast<T*>(_prev);
    }

    const T* prev() const
    {
        return static_cast<const T*>(_prev);
    }
};

template <typename T, int N, typename PTR, typename Next, typename Prev>
class ListIterator
{
    PTR ptr;

    ListIterator(PTR p) : ptr(p) {}
    friend class List<T, N>;

public:
    friend bool operator == (const ListIterator& it1, const ListIterator& it2)
    {
        return it1.ptr == it2.ptr;
    }

    friend bool operator != (const ListIterator& it1, const ListIterator& it2)
    {
        return it1.ptr != it2.ptr;
    };

    T& val()
    {
        return *const_cast<T*>(static_cast<const T*>(ptr));
    }

    const T& val() const
    {
        return *const_cast<T*>(static_cast<const T*>(ptr));
    }

    T& operator * ()
    {
        return val();
    }

    const T& operator * () const
    {
        return val();
    }

    T* operator -> ()
    {
        return &val();
    }

    const T* operator -> () const
    {
        return &val();
    }

    ListIterator& operator ++ ()
    {
        ptr = Next()(ptr);
        return *this;
    }

    ListIterator operator ++ (int)
    {
        ListIterator tmp = *this;
        ptr = Next()(ptr);
        return tmp;
    }

    ListIterator& operator -- ()
    {
        ptr = Prev()(ptr);
        return *this;
    }

    ListIterator operator -- (int)
    {
        ListIterator tmp = *this;
        ptr = Prev()(ptr);
        return tmp;
    }

    operator T* ()
    {
        return &val();
    }

    operator const T* () const
    {
        return &val();
    }
};

template <typename T, int N>
class List
{
    struct IteratorNext
    {
        ListNode<T, N>* operator () (ListNode<T, N>* ptr) const
        {
            return ptr->next();
        }

        const ListNode<T, N>* operator () (const ListNode<T, N>* ptr) const
        {
            return ptr->next();
        }
    };

    struct IteratorPrev
    {
        ListNode<T, N>* operator () (ListNode<T, N>* ptr) const
        {
            return ptr->prev();
        }

        const ListNode<T, N>* operator () (const ListNode<T, N>* ptr) const
        {
            return ptr->prev();
        }
    };

public:
    typedef ListIterator<T, N, ListNode<T, N>*, IteratorNext, IteratorPrev> Iterator;
    typedef ListIterator<T, N, ListNode<T, N>*, IteratorPrev, IteratorNext> RIterator;

    typedef ListIterator<T, N, const ListNode<T, N>*, IteratorNext, IteratorPrev> CIterator;
    typedef ListIterator<T, N, const ListNode<T, N>*, IteratorPrev, IteratorNext> RCIterator;

private:
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

    bool empty() const
    {
        bool isEmpty = head._prev == &head;
        if (isEmpty)
            assert(head._prev == &head && head._next == &head);
        return isEmpty;
    }

    Iterator begin()
    {
        return Iterator(head.next());
    }

    CIterator begin() const
    {
        return CIterator(head.next());
    }

    RIterator rbegin()
    {
        return RIterator(head.prev());
    }

    RCIterator rbegin() const
    {
        return RCIterator(head.prev());
    }

    Iterator end()
    {
        return Iterator(&head);
    }

    CIterator end() const
    {
        return CIterator(&head);
    }

    RIterator rend()
    {
        return RIterator(&head);
    }

    RCIterator rend() const
    {
        return RCIterator(&head);
    }
};

template <typename T, int N>
inline ListNode<T, N>::ListNode(List<T, N>& list)
{
    static_assert(std::is_base_of<ListNode<T, N>, T>::value, "class ListNode<T, N> must base of T");
    list.pushBack(*static_cast<T*>(this));
}
