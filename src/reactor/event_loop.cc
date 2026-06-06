#include "../include/reactor/event_loop.h"
#include <cstdint>
#include <cstdio>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

namespace simple{
EventLoop::EventLoop(Acceptor& acceptor)
    : _evtList{1024}
    , _acceptor(acceptor)
    , _epfd(CreateEpollFd())
    , _evtfd{CreateEventFd()}
    , _is_looping(false){
        AddEpollReadFd(acceptor.Fd());
        AddEpollReadFd(_evtfd);
    }

EventLoop::~EventLoop(){
    if(_epfd){
        close(_epfd);
    }

    if(_evtfd){
        close(_evtfd);
    }
}

void EventLoop::Loop(){
    _is_looping = true;
    while(_is_looping){
        WaitEpollFd();
    }
}

void EventLoop::Unloop(){
    _is_looping = false;
}

void EventLoop::RunInLoop(Functor&& cb){
    {
        MutexLockGuard auto_lock(_mutex);
        _pengings_callback.push_back(std::move(cb));
    }
    WakeUp();
}

void EventLoop::DoPengingFunctors(){
    vector<Functor> tmp;
    {
        MutexLockGuard auto_lock(_mutex);
        tmp.swap(_pengings_callback);
    }
    for(auto& cb : tmp){
        cb();
    }
}

ssize_t EventLoop::WakeUp() const{
    uint64_t one = 1;
    ssize_t ret = ::write(_evtfd, &one, sizeof(one));
    if(ret != sizeof(one)){
        perror("write");
    }
    return ret;
}

ssize_t EventLoop::HandleRead() const{
    uint64_t one = 1;
    ssize_t ret = ::read(_evtfd, &one, sizeof(one));
    if(ret != sizeof(one)){
        perror("read");
    }
    return ret;
}

void EventLoop::WaitEpollFd(){
    int ready_num = -1;
    do{
        ready_num = ::epoll_wait(_epfd, &*_evtList.begin(), static_cast<int>(_evtList.size()), 5000);
    } while (-1 == ready_num && errno == EINTR);

    if(-1 == ready_num){
        perror("ready_num == -1");
        return;
    }
    if(0 == ready_num){
        printf(">>epoll_wait timeout\n");
    } else {
        if(ready_num == static_cast<int>(_evtList.size())){
            _evtList.resize(2 * static_cast<uint64_t>(ready_num));
        }

        for(size_t idx = 0; static_cast<int>(idx) < ready_num; ++idx){
            int fd = _evtList[idx].data.fd;
            if(fd == _acceptor.Fd()){
                if(_evtList[idx].events & EPOLLIN){
                    HandleNewConnection();
                }
            } else if(fd == _evtfd){
                HandleRead();
                DoPengingFunctors();
            } else {
                if(_evtList[idx].events & EPOLLIN){
                    HandleMessage(fd);
                }
            }
        }
    }
}

void EventLoop::HandleNewConnection(){
    int peerfd = _acceptor.Accept();
    AddEpollReadFd(peerfd);

    TcpConnectionPtr con(new TcpConnection(peerfd, this));

    con->SetConnectionCallback(_on_connection_callback);
    con->SetMessageCallback(_on_message_callback);
    con->SetCloseCallback(_on_close_callback);

    _conns.insert(std::make_pair(peerfd, con));
    con->HandleConnectionCallback();
}

void EventLoop::HandleMessage(int fd){
    auto iter = _conns.find(fd);
    if(iter != _conns.end()){
        bool flag = iter->second->IsClosed();
        if(flag){
            iter->second->HandleCloseCallback();
            DelEpollReadFd(fd);
            _conns.erase(iter);
        } else {
            iter->second->HandleMessageCallback();
        }
    } else {
        printf("连接不存在\n");
    }
}

int EventLoop::CreateEpollFd(){
    int fd = epoll_create(100);
    if(-1 == fd){
        perror("epoll_create");
    }
    return fd;
}

int EventLoop::AddEpollReadFd(int fd) const{
    struct epoll_event evs;
    evs.events = EPOLLIN;
    evs.data.fd = fd;

    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &evs);
    if(ret == -1){
        perror("EPOLL_CTL_ADD");
    }
    return ret;
}

int EventLoop::DelEpollReadFd(int fd) const{
    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;

    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);
    if(ret == -1){
        perror("epoll_ctl del");
    }
    return ret;
}

void EventLoop::SetConnectionCallback(TcpConnectionCallback&& cb){
    _on_connection_callback = std::move(cb);
}

void EventLoop::SetCloseCallback(TcpConnectionCallback&& cb){
    _on_close_callback = std::move(cb);
}

void EventLoop::SetMessageCallback(TcpConnectionCallback&& cb){
    _on_message_callback = std::move(cb);
}

int EventLoop::CreateEventFd(){
    int fd = eventfd(10, 0);
    if(-1 == fd){
        perror("eventfd");
    }
    return fd;
}
}
