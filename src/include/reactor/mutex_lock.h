#ifndef __MUTEX_LOCK_H__
#define __MUTEX_LOCK_H__
#include <pthread.h>
#include <cstdio>
namespace simple{
class MutexLock{
public:
    MutexLock();
    ~MutexLock();
    MutexLock(const MutexLock&) = delete;
    MutexLock& operator=(const MutexLock&) = delete;
    MutexLock(MutexLock&&) = default;
    MutexLock& operator=(MutexLock&&) = default;

    int Lock();
    int Unlock();

    pthread_mutex_t* GetMutexLockPtr(){
        return &_mutex;
    }

private:
    pthread_mutex_t _mutex;
};

class MutexLockGuard{
public:
    explicit MutexLockGuard(MutexLock& mutex) : _mutex(mutex){
        _mutex.Lock();
    }

    ~MutexLockGuard(){
        _mutex.Unlock();
    }

private:
    MutexLock& _mutex;
};
}
#endif
