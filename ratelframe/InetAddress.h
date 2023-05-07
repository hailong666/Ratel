#pragma once

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
// 封装socket地址类型

class InetAddress
{
private:
    sockaddr_in addr_;
public:
    explicit InetAddress(uint16_t port = 0, std::string ip = "0.0.0.0");
    explicit InetAddress(const sockaddr_in &addr)
        : addr_(addr)
    {

    }
    std::string toIp() const;
    std::string toIpPort() const;
    uint16_t toPort() const;
    void setSockAddr(const sockaddr_in &addr){addr_ = addr;}
    const sockaddr_in* getSockAddr() const {return &addr_;}
};

