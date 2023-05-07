#include <mymuduo/TcpServer.h>
#include <mymuduo/Logger.h>
#include <string>

class EchoServer
{
public:
    EchoServer(EventLoop *loop,
            const InetAddress &addr,
            const std::string &name)
            : server_(loop, addr, name)
            , loop_(loop)
    {
        // 注册回调你函数
        server_.setConnectionCallback(
            std::bind(&EchoServer::onConnection, this, std::placeholders::_1)
        );

        server_.setMessageCallback(
            std::bind(&EchoServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
        );
        //设置合适的Loop线程数量
        server_.setThreadNum(3);
    }
    void start()
    {
        server_.start();
    }
private:
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())    
        {
            LOG_INFO("Connction UP : %s", conn->peerAddress().toIpPort().c_str());
        }
        else
        {
            LOG_INFO("Connction Down : %s", conn->peerAddress().toIpPort().c_str());
        }
    }

    // 可读事件回调
    void onMessage(const TcpConnectionPtr &conn,
                    Buffer *buf,
                    Timestamp time)
    {
        std::string msg = buf->retrieveAllString();
        conn->send(msg);
        conn->shutdown(); // 写端  EPOLLHUP ->  closeCallback_
    }

    EventLoop *loop_;
    TcpServer server_;
};
int main()
{
    EventLoop loop;
    InetAddress addr(8000);
    EchoServer server(&loop, addr, "EchoServer-01"); // Acceptor non-blocking listenfd  create bind
    server.start(); // listen loopthread listenfd -> acceptChannel -> mainLoop ->
    loop.loop(); //启动mainLoop的底层Poller

    return 0;
}