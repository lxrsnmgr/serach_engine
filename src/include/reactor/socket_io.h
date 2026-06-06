#ifndef __SOCKET_IO_H__
#define __SOCKET_IO_H__
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>

namespace simple{
class SocketIO{
public:
    explicit SocketIO(int fd);
    ssize_t Readn(char* buf, size_t len) const;
    ssize_t ReadLine(char* buf, size_t len) const;
    ssize_t Writen(const char* buf, size_t len) const;

private:
    int _fd;
};
}
#endif
