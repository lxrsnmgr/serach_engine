#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "acceptor.h"
#include "event_loop.h"
#include "tcp_connection.h"
#include <iostream>
#include <unistd.h>

namespace simple{
class TcpServer{
public:
    TcpServer(const string& ip, unsigned short port)
        : _acceptor(ip, port), _loop(_acceptor){

        }

    void Start(){
        _acceptor.Ready();
        _loop.Loop();
    }

    void Stop(){
        _loop.Unloop();
    }

    void SetAllCallback(TcpConnectionCallback&& onConnection,
                        TcpConnectionCallback&& onMessage,
                        TcpConnectionCallback&& onClose){
        _loop.SetConnectionCallback(std::move(onConnection));
        _loop.SetMessageCallback(std::move(onMessage));
        _loop.SetCloseCallback(std::move(onClose));
    }

private:
    Acceptor _acceptor;
    EventLoop _loop;
};
}
#endif
