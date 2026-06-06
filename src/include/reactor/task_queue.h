#ifndef __TASK_QUEUE_H__
#define __TASK_QUEUE_H__

#include "../reactor/condition.h"
#include "../reactor/mutex_lock.h"
#include "../reactor/task.h"
#include <queue>

namespace simple{
using std::queue;

using Elem = function<void()>;

class TaskQueue{
public:
    explicit TaskQueue(size_t queue_size);
    bool Empty() const;
    bool Full() const;
    void Push(Elem&& value);
    Elem Pop();

    void Wakeup();

private:
    queue<Elem> _que;
    Condition _not_empty;
    Condition _not_full;
    MutexLock _mutex;
    size_t _queue_size;
    bool _flag;
};
}
#endif
