#include "../include/reactor/thread.h"
#include <pthread.h>
#include <cstdio>


namespace simple{
Thread::Thread(ThreadCallback &&cb)
    : _cb(std::move(cb)), _thid(0), _is_running(false){

    }

Thread::~Thread(){
    if(_is_running){
        int ret = pthread_detach(_thid);
    }
}

int Thread::Start(){
    int ret = pthread_create(&_thid, nullptr, ThreadFunc, this);
    if(ret){
        perror("pthread_creae");
        return ret;
    }

    _is_running = true;
    return 0;
}

int Thread::Join(){
    int ret = 0;
    if(_is_running){
        ret = pthread_join(_thid, nullptr);
        _is_running = false;
    }
    return ret;
}

void* Thread::ThreadFunc(void* arg){
    auto* pth = static_cast<Thread*>(arg);
    if(pth){
        pth->_cb();
    }
    pthread_exit(nullptr);
}
}
