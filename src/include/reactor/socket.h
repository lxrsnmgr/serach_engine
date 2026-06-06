#ifndef __SOCKET_H__
#define __SOCKET_H__
class Socket{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    Socket(Socket &&) = default;
    Socket& operator=(Socket&&) = default;

    int Fd() const;
    int ShutDownWrite() const;

private:
    int _fd;
};
#endif
