#pragma once

#include <cstdint>

#include "List.h"
#include "common.h"

enum class TaskStatus : uint16_t
{
    RUNNING, WAITING
};

class TaskStatusInfo;

typedef List<TaskStatusInfo, 0> TaskStatusList;

class TaskStatusInfo : public ListNode<TaskStatusInfo, 0>
{
public:
    class Locker
    {
        TaskStatusInfo* info;
        Locker(TaskStatusInfo* _info) : info(_info)
        {
            assert(info != nullptr);
            info->lock();
        }

        friend class TaskStatusInfo;

    public:
        Locker() = delete;
        Locker(const Locker&) = delete;
        Locker& operator = (const Locker&) = delete;
        Locker& operator = (Locker&&) = delete;

        Locker(Locker&& locker) : info(locker.info)
        {
            locker.info = nullptr;
        }

        ~Locker()
        {
            if (info != nullptr)
                info->unlock();
        }
    };

private:
	TaskStatus status;
    uint16_t lockCount;

protected:
	TaskStatusInfo();

public:
	typedef List<TaskStatusInfo, 0> StatusList;

	TaskStatusInfo(unsigned long options, const TaskStatusInfo& ts)
        : ListNode<TaskStatusInfo, 0>(const_cast<TaskStatusInfo*>(&ts)), status(ts.status), lockCount(ts.lockCount)
    {
        assert(status == TaskStatus::RUNNING);
    }

    TaskStatusInfo(const TaskStatusInfo&) = delete;
    TaskStatusInfo(TaskStatusInfo&&) = delete;
    TaskStatusInfo& operator = (const TaskStatusInfo&) = delete;
    TaskStatusInfo& operator = (TaskStatusInfo&&) = delete;

    void changeStatus(TaskStatus s, TaskStatusList& list)
    {
    	removeSelf();
    	list.pushBack(*this);
    	status = s;

    }

    bool equal(TaskStatus s)
    {
    	return status == s;
    }

    TaskStatus curState()
    {
    	return status;
    }

    void lock()
    {
        if (lockCount == 0)
            _cli();
        ++lockCount;
    }

    void unlock()
    {
        assert(lockCount > 0);
        if (lockCount == 1)
            _sti();
        --lockCount;
    }

    void vLock()
    {
        ++lockCount;
    }

    void vUnlock()
    {
        assert(lockCount > 0);
        --lockCount;
    }

    Locker getLocker()
    {
        return Locker(this);
    }

    uint32_t curLockCount() const
    {
        return lockCount;
    }
};

