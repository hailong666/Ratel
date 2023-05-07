#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include "Logger.h"
#include "Acceptor.h"
#include "InetAddress.h"
static int createNonblocking()
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0)
    {
        LOG_FATAL("%s:%s:%d listen socket create err:%d \n", __FILE__, __FUNCTION__, __LINE__, errno );
    }
    return sockfd;
}

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reuseport)
                : loop_(loop)
                , acceptSocket_(createNonblocking())
                , acceptChannel_(loop, acceptSocket_.fd())
                , listenning_(false)
{
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.setReusePort(true);
    acceptSocket_.bindAddress(listenAddr);
    // baseloop 监听到有新用户连 则调用handleRead函数去处理
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}
Acceptor::~Acceptor()
{
    acceptChannel_.disableAll();
    acceptChannel_.remove();
}

void Acceptor::listen()
{
    listenning_ = true;
    acceptSocket_.listen();  // listen
    acceptChannel_.enableReading();

}
// listenfd 有事件发生，也就是有新用户连接了
void Acceptor::handleRead()
{
    InetAddress peerAddr;
    int connfd = acceptSocket_.accept(&peerAddr);
    if (connfd >= 0)
    {
        if (newConnectionCallback_)
        {
            newConnectionCallback_(connfd, peerAddr);  // 轮询找到subLoop， 唤醒，分发当前新客户端的Channel
        }
        else
        {
            ::close(connfd);
            LOG_ERROR("%s:%s:%d accept err:%d \n", __FILE__, __FUNCTION__, __LINE__, errno );
            if (errno == EMFILE)
            {
                LOG_ERROR("%s:%s:%d spcket reach full err:%d \n", __FILE__, __FUNCTION__, __LINE__, errno );
            }
            
        }
    }
}