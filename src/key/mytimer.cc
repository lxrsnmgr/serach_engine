#include "../include/key/mytimer.h"
#include <cstdint>
#include <sys/select.h>

namespace simple{
MyTimer::MyTimer(){
    _flag = false;
    gettimeofday(&_beginTime, nullptr);
}

MyTimer::MyTimer(int num)
: _num(num)
, _totaltime(0){
    _flag = true;
}

MyTimer::~MyTimer(){
    if(_flag == false){
        gettimeofday(&_endTime, nullptr);
        printf("total time = %ld us\n", (_endTime.tv_sec - _beginTime.tv_sec) * 1000000 + _endTime.tv_usec - _beginTime.tv_usec);
    }
}

void MyTimer::start(){
    gettimeofday(&_beginTime, nullptr);
}

void MyTimer::stop(){
    gettimeofday(&_endTime, nullptr);
}

__int64_t MyTimer::getTime() const{
    return _totaltime;
}

void MyTimer::printTime() const{
    printf("total time = %ld us\n", _totaltime);
}
}
