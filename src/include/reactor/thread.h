#ifndef __THREAD_H__
#define __THREAD_H__
#include <functional>
#include <pthread.h>
namespace simple{
using std::function;

using ThreadCallback = function<void()>;

class Thread{
public:
    explicit Thread(ThreadCallback &&cb);
    ~Thread();
    int Start();
    int Join();

private:
    //线程入口函数
    static void* ThreadFunc(void* arg);

    ThreadCallback _cb;
    pthread_t _thid;
    bool _is_running;
};
}
#endif
