#include "../include/reactor/mutex_lock.h"

namespace simple{
MutexLock::MutexLock(){
    int ret = pthread_mutex_init(&_mutex, nullptr);
    if(ret){
        perror("pthread_mutex_init");
    }
}

MutexLock::~MutexLock(){
    int ret = pthread_mutex_destroy(&_mutex);
    if(ret){
        perror("pthread_mutex_destroy");
    }
}

int MutexLock::Lock(){
    int ret = pthread_mutex_lock(&_mutex);
    if(ret){
        perror("pthread_mutex_lock");
    }
    return ret;
}

int MutexLock::Unlock(){
    int ret = pthread_mutex_unlock(&_mutex);
    if(ret){
        perror("pthread_mutex_unlock");
    }
    return ret;
}
}
