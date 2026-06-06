#include "../include/reactor/inet_address.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>

namespace simple{
InetAddress::InetAddress(const string& ip, unsigned short port){
    memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

InetAddress::InetAddress(const struct sockaddr_in& addr)
: _addr{addr}{

}

string InetAddress::Ip() const{
    return string(inet_ntoa(_addr.sin_addr));
}

uint16_t InetAddress::Port() const{
    return ntohs(_addr.sin_port);
}

const sockaddr_in* InetAddress::GetInetAddrPtr() const{
    return &_addr;
}
}
