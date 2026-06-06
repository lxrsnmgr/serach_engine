#ifndef __CONDITION_H__
#define __CONDITION_H__

#include <pthread.h>

namespace simple{
class MutexLock;

class Condition{
public:
    explicit Condition(MutexLock& mutex);
    ~Condition();
    Condition(const Condition&) = delete;
    Condition& operator=(const Condition&) = delete;
    Condition(Condition&&) = default;

    int Wait();
    int Notify();
    int NotifyAll();

private:
    pthread_cond_t _cond;
    MutexLock& _mutex;
};
}
#endif
