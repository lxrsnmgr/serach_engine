#include "../include/reactor/tcp_connection.h"
#include "../include/reactor/event_loop.h"
#include <iostream>
#include <sstream>

namespace simple{
using std::ostringstream;

TcpConnection::TcpConnection(int fd, EventLoop* loop)
    : _loop(loop)
    , _sock(fd)
    , _sock_io(fd)
    , _local_addr(GetLocalAddr())
      , _peer_addr(GetPeerAddr()){

      }

void TcpConnection::Send(const string& msg){
    _sock_io.Writen(msg.c_str(), msg.size());
}

void TcpConnection::SendInLoop(const string& msg){
    if(_loop){
        _loop->RunInLoop([this, msg]{Send(msg);});
    }
}

string TcpConnection::Receive(){
    char buff[65535] = {0};
    _sock_io.ReadLine(buff, sizeof(buff));

    return string(buff);
}

string TcpConnection::ToString(){
    ostringstream oss;
    oss << _local_addr.Ip() << ":" << _local_addr.Port() << "--->"
        << _peer_addr.Ip() << ":" << _peer_addr.Port();
    return oss.str();
}

string TcpConnection::GetPeerAddrString(){
    ostringstream oss;
    oss << _peer_addr.Ip() << ":" << _peer_addr.Port();
    return oss.str();
}

bool TcpConnection::IsClosed() const{
    char buf[10] = {};
    ssize_t ret = ::recv(_sock.Fd(), buf, sizeof(buf), MSG_PEEK);

    return (ret == 0);
}

InetAddress TcpConnection::GetLocalAddr(){
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret = getpeername(_sock.Fd(), reinterpret_cast<struct sockaddr*>(&addr), &len);
    if(-1 == ret){
        perror("getpeername");
    }
    return InetAddress(addr);
}

void TcpConnection::SetConnectionCallback(const TcpConnectionCallback& cb){
    _on_connection_callback = std::move(cb);
}

void TcpConnection::SetMessageCallback(const TcpConnectionCallback& cb){
    _on_message_callback = std::move(cb);
}

void TcpConnection::SetCloseCallback(const TcpConnectionCallback& cb){
    _on_close_callback = std::move(cb);
}

void TcpConnection::HandleConnectionCallback(){
    if(_on_connection_callback){
        _on_connection_callback(shared_from_this());
    }
}

void TcpConnection::HandleMessageCallback(){
    if(_on_message_callback){
        _on_message_callback(shared_from_this());
    }
}

void TcpConnection::HandleCloseCallback(){
    if(_on_close_callback){
        _on_close_callback(shared_from_this());
    }
}
}
