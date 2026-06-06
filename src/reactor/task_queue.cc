#include "../include/reactor/task_queue.h"

namespace simple{
TaskQueue::TaskQueue(size_t queue_size)
: _not_empty(_mutex)
, _not_full(_mutex)
, _queue_size(queue_size){

}

bool TaskQueue::Empty() const{
    return _que.empty();
}

bool TaskQueue::Full() const{
    return _que.size() == _queue_size;
}

void TaskQueue::Push(Elem&& value){
    MutexLockGuard auto_lock(_mutex);

    while(Full()){
        _not_full.Wait();
    }
    _que.push(std::move(value));
    _not_empty.Notify();
}

Elem TaskQueue::Pop(){
    MutexLockGuard auto_lock(_mutex);

    while(_flag && Empty()){
        _not_empty.Wait();
    }

    if(_flag){
        Elem tmp = _que.front();
        _que.pop();

        _not_full.Notify();
        return tmp;
    }
    return nullptr;
}

void TaskQueue::Wakeup(){
    _flag = false;
    _not_empty.NotifyAll();
}
}
