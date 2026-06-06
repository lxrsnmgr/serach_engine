#ifndef __INET_ADDRESS_H__
#define __INET_ADDRESS_H__

#include <arpa/inet.h>
#include <string>

namespace simple{
using std::string;

class InetAddress{
public:
    InetAddress(const string& ip, unsigned short port);
    explicit InetAddress(const struct sockaddr_in& addr);
    string Ip() const;
    uint16_t Port() const;
    const sockaddr_in* GetInetAddrPtr() const;

private:
    struct sockaddr_in _addr;
};
}
#endif
