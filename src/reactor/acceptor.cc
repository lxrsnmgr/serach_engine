#include "../include/reactor/acceptor.h"
#include <asm-generic/socket.h>
#include <sys/socket.h>

namespace simple{
Acceptor::Acceptor(const string& ip, unsigned short port)
    : _serv_addr{ip, port}{

    }

int Acceptor::Ready(){
    int ret = 0;
    ret = SetReuseAddr();
    if(ret == -1){
        return -1;
    }
    ret = SetReusePort();
    if(ret == -1){
        return -1;
    }
    ret = Bind();
    if(ret == -1){
        return -1;
    }
    ret = Listen();
    if(ret == -1){
        return -1;
    }
    return 0;
}

int Acceptor::SetReuseAddr(){
    int on = 1;
    int ret = setsockopt(_listen_sock.Fd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if(ret){
        perror("setsockopt");
    }
    return ret;
}

int Acceptor::SetReusePort(){
    int on = 1;
    int ret = setsockopt(_listen_sock.Fd(), SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
    if(-1 == ret){
        perror("setsockopt");
    }
    return ret;
}

int Acceptor::Bind(){
    int ret = ::bind(_listen_sock.Fd(), reinterpret_cast<sockaddr*>(
                                            const_cast<sockaddr_in*>(_serv_addr.GetInetAddrPtr())),
                                            sizeof(struct sockaddr));
    if(ret == -1){
        perror("bind");
    }
    return ret;
}

int Acceptor::Listen(){
    int ret = ::listen(_listen_sock.Fd(), 128);
    if(-1 == ret){
        perror("listen");
    }
    return ret;
}

int Acceptor::Accept(){
    int connfd = ::accept(_listen_sock.Fd(), nullptr, nullptr);
    if(-1 == connfd){
        perror("listen");
    }
    return connfd;
}

int Acceptor::Fd() const{
    return _listen_sock.Fd();
}
}
