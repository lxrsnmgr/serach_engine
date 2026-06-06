#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "thread.h"
#include "task_queue.h"
#include <algorithm>
#include <memory>
#include <vector>

namespace simple{
using std::unique_ptr;
using std::vector;

class ThreadPool{
public:
    ThreadPool(size_t thread_num, size_t queue_size);
    ~ThreadPool();

    void Start();
    void Stop();

    void AddTask(Task&& task);
    Task GetTask();

private:
    void ThreadFunc();

    TaskQueue _task_queue;
    vector<unique_ptr<Thread>> _threads;
    size_t _thread_num;
    bool _isExit;
};
}
#endif
