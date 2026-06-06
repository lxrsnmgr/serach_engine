#include "../include/reactor/thread_pool.h"
#include <chrono>
#include <thread>
#include <unistd.h>

namespace simple{
ThreadPool::ThreadPool(size_t thread_num, size_t queue_size)
: _task_queue(queue_size)
, _thread_num(thread_num)
    , _isExit(false){
        _threads.reserve(_thread_num);
    }

ThreadPool::~ThreadPool(){
    if(!_isExit){
        Stop();
        _isExit = true;
    }
}

void ThreadPool::Start(){
    for(size_t idx = 0; idx < _thread_num; ++idx){
        unique_ptr<Thread> up(new Thread([this]{ThreadFunc();}));
        _threads.push_back(std::move(up));
    }

    for(auto& th : _threads){
        th->Start();
    }
}

void ThreadPool::AddTask(Task&& task){
    if(task){
        _task_queue.Push(std::move(task));
    }
}

Task ThreadPool::GetTask(){
    return _task_queue.Pop();
}

void ThreadPool::Stop(){
    while(!_task_queue.Empty()){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    _isExit = true;
    _task_queue.Wakeup();
    for(auto& th : _threads){
        th->Join();
    }
}

void ThreadPool::ThreadFunc(){
    while(!_isExit){
        Task taskcb = GetTask();
        if(taskcb){
            taskcb();
        }
    }
}
}
