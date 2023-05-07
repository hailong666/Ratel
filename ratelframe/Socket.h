#pragma once

#include "noncopyable.h"

class InetAddress;

class Socket
{
private:
    const int sockfd_;    
public:
    explicit Socket(int sockfd):sockfd_(sockfd){}

    int fd() const {return sockfd_;}
    void bindAddress(const InetAddress &localaddr);
    void listen();
    int accept(InetAddress *peeraddr);

    void shutdownWrite();

    void setTcpNoDelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);

   
    ~Socket();
};

