#include "InetAddress.h"
#include <strings.h>
#include <cstdio>
#include <cstdlib>
InetAddress::InetAddress(uint16_t port, std::string ip)
{
    bzero(&addr_, sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
}

std::string InetAddress::toIp() const
{
    char buf[64];
    ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof buf);
    return buf;
}
std::string InetAddress::toIpPort() const
{
    //ip:port
    std::string ip = toIp();
    char buf[64];
    sprintf(buf, "%s:%u", ip.c_str(), toPort());
    return buf;
}
uint16_t InetAddress::toPort() const
{
    uint16_t port = ntohs(addr_.sin_port);
    return port;
}

// #include <iostream>

// int main(){

//     InetAddress addr(8080, "43.168.69.4");
//     std::cout << addr.toIpPort() << std::endl;
//     return 0;
// }