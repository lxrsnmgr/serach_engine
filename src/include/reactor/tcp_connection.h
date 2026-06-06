#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__

#include "inet_address.h"
#include "socket.h"
#include  "socket_io.h"
#include <functional>
#include <memory>

namespace simple{
using std::function;
using std::shared_ptr;

class EventLoop;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>{
public:
    using TcpConnectionPtr = shared_ptr<TcpConnection>;
    using TcpConnectionCallback = function<void(const TcpConnectionPtr&)>;
    TcpConnection(int fd, EventLoop* loop);
    void Send(const string& msg);
    void SendInLoop(const string& msg);
    string Receive();
    string ToString();
    string GetPeerAddrString();
    bool IsClosed() const;

    //注册三个事件的回调函数
    void SetConnectionCallback(const TcpConnectionCallback& cb);
    void SetMessageCallback(const TcpConnectionCallback& cb);
    void SetCloseCallback(const TcpConnectionCallback& cb);

    //三个事件回调函数的执行
    void HandleConnectionCallback();
    void HandleMessageCallback();
    void HandleCloseCallback();

private:
    InetAddress GetLocalAddr();
    InetAddress GetPeerAddr();

    TcpConnectionCallback _on_connection_callback;
    TcpConnectionCallback _on_message_callback;
    TcpConnectionCallback _on_close_callback;
    EventLoop* _loop;
    Socket _sock;
    SocketIO _sock_io;
    InetAddress _local_addr;
    InetAddress _peer_addr;
};
}
#endif
