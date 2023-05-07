#pragma once

/**
 * 用户使用muduo编写服务器程序，所以我们这里包含需要的头文件 避免用户使用的适合包含很多
*/
#include <functional>
#include <string>
#include <atomic>
#include <unordered_map>

#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include "Buffer.h"
#include "InetAddress.h"
#include "noncopyable.h"
#include "EventLoopThreadPool.h"
#include "Callbacks.h"
// 对外的服务器编程使用的类

class TcpServer : noncopyable
{
public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;
    
    enum Option
    {
        kNoReusePort,
        kReusePort,
    };

    TcpServer(EventLoop *loop, const InetAddress &listenAddr,const std::string &nameArg , Option option = kNoReusePort);
    ~TcpServer();    
    void setThreadInitcallback(const ThreadInitCallback &cb){threadInitCallback_ = cb;}
    void setConnectionCallback(const ConnectionCallback &cb){ connectionCallback_ = cb;}
    void setMessageCallback(const MessageCallback &cb){ messageCallback_ = cb;}
    void setWriteCompleteCallbck(const WriteCompleteCallback &cb){ writeCompleteCallback_ = cb;}

    void setThreadNum(int numThreads); // 设置底层subloop个数

    void start(); // 开启服务器监听
private:
    void newConnection(int sockfd, const InetAddress &peerAddr);
    void removeConnection(const TcpConnectionPtr &conn);
    void removeConnectionInLoop(const TcpConnectionPtr &conn);

    using ConnectionMap = std::unordered_map<std::string, TcpConnectionPtr>;
    EventLoop *loop_; // baseLoop 用户定义的

    const std::string ipPort_;
    const std::string name_;

    std::unique_ptr<Acceptor> acceptor_; // mainLoop 中的监听新连接事件

    std::shared_ptr<EventLoopThreadPool> threadPool_;

    ConnectionCallback connectionCallback_;  // 新连接回调
    MessageCallback messageCallback_; // 读写消息回调
    WriteCompleteCallback writeCompleteCallback_; // 消息发送完成以后的回调

    ThreadInitCallback threadInitCallback_; // loop线程初始化回调

    std::atomic_int started_;

    int nextConnId_;
    ConnectionMap  connections_;
};


