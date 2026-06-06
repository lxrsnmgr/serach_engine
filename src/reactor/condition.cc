#include "../include/reactor/condition.h"
#include "../include/reactor/mutex_lock.h"
#include <pthread.h>

namespace simple{
Condition::Condition(MutexLock& mutex)
: _mutex(mutex)
{
    int ret = pthread_cond_init(&_cond, nullptr);
    if(ret){
        perror("pthread_cond_init");
    }
}

Condition::~Condition(){
    int ret = pthread_cond_destroy(&_cond);
    if(ret){
        perror("pthread_cond_destroy");
    }
}

int Condition::Wait(){
    int ret = pthread_cond_wait(&_cond, _mutex.GetMutexLockPtr());
    if(ret){
        perror("pthread_cond_wait");
    }
    return ret;
}

int Condition::Notify(){
    int ret = pthread_cond_signal(&_cond);
    if(ret){
        perror("pthread_cond_signal");
    }
    return ret;
}

int Condition::NotifyAll(){
    int ret = pthread_cond_broadcast(&_cond);
    if(ret){
        perror("pthread_cond_broadcast");
    }
    return ret;
}
}
