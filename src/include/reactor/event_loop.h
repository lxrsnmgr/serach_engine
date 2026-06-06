#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "mutex_lock.h"
#include "acceptor.h"
#include "tcp_connection.h"
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <memory>
#include <functional>

namespace simple{
using std::vector;
using std::map;
using std::shared_ptr;
using std::function;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr&)>;
using Functor = function<void()>;

class EventLoop{
public:
    explicit EventLoop(Acceptor &acceptor);
    ~EventLoop();
    void Loop();
    void Unloop();

    void RunInLoop(Functor &&cb);

    ssize_t WakeUp() const;
    ssize_t HandleRead() const;

    void DoPengingFunctors();

    void SetConnectionCallback(TcpConnectionCallback &&cb);
    void SetMessageCallback(TcpConnectionCallback &&cb);
    void SetCloseCallback(TcpConnectionCallback &&cb);

private:
    void WaitEpollFd();
    void HandleNewConnection();
    void HandleMessage(int fd);
    static int CreateEpollFd();
    int AddEpollReadFd(int fd) const;
    int DelEpollReadFd(int fd) const;
    static int CreateEventFd();

    map<int, TcpConnectionPtr> _conns;
    MutexLock _mutex;
    TcpConnectionCallback _on_connection_callback;
    TcpConnectionCallback _on_message_callback;
    TcpConnectionCallback _on_close_callback;
    vector<struct epoll_event> _evtList;
    vector<Functor> _pengings_callback;
    Acceptor &_acceptor;
    int _epfd;
    int _evtfd;
    bool _is_looping;
};
}
#endif

