#ifndef __MYTIMER_H__
#define __MYTIMER_H__
#include <unistd.h>
#include <sys/time.h>
#include <cstdio>

namespace simple{
class MyTimer{
public:
    MyTimer();
    explicit MyTimer(int num);
    ~MyTimer();
    void start();
    void stop();
    __int64_t getTime() const;
    void printTime() const;

private:
    int _num;
    bool _flag;
    struct timeval _beginTime;
    struct timeval _endTime;
    __int64_t _totaltime;
};
}
#endif
