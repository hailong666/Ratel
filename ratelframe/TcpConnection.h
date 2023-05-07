#pragma once

#include <memory>
#include <string>
#include <atomic>
#include "Buffer.h"
#include "Callbacks.h"
#include "InetAddress.h"
#include "noncopyable.h"
#include "Timestamp.h"

class Channel;
class EventLoop;
class Socket;

/**
 * 
 * TcpServer => Acceptor  => 有一个新用户连接，通过accept函数拿到connfd
 * 
 * => TcpConnection 设置回调 =》Channel => Poller => Channel的回调操作
*/

class TcpConnection :noncopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop *loop, 
                    const std::string &nameArg, 
                    int sockfd, 
                    const InetAddress& localAddr,
                    const InetAddress& peerAddr);
    ~TcpConnection();   

    EventLoop* getLoop() const { return loop_;} 
    const std::string& name() const {return name_;}
    const InetAddress& localAddress() const { return localAddr_;}
    const InetAddress& peerAddress() const { return peerAddr_;}

    bool connected() const { return state_ == kConnected;}
    bool disconnected() const { return state_ == kDisconnected;}

    // 发送数据
    // void send(const void *message, int len);
    // 关闭数据
    void shutdown();

    void setConnectionCallback(const ConnectionCallback& cb) { connectionCallback_ = cb;}
    void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb;}
    void setWriteCompleteCallback(const WriteCompleteCallback& cb) { writeCompleteCallback_ = cb;}
    void setHighWaterCallback(const HighWaterMarkCallback& cb, size_t highWaterMark) {highWaterMarkCallback_ = cb ,highWaterMark_ = highWaterMark;}

    void setCloseCallback(const CloseCallback &cb) {closeCallback_ = cb;}

    // 连接建立
    void connectEstablished();
    // 连接销毁
    void connectDestroyed();

    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();

    void sendInLoop(const void* data, size_t len);
    void shutdownInLoop();

    // 发送数据
    void send(const std::string &buf);
    
private:
    enum StateE {kDisconnected, kConnecting, kConnected, kDisconnecting};
    void setState(StateE state){ state_ = state;}
    EventLoop *loop_; //这里绝对不是baseloop， 因为TcpConnection都是在subloop中管理的
    const std::string name_;
    std::atomic_int state_;
    bool reading_;

    // 这里和Acceptor类似
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;

    const InetAddress localAddr_;
    const InetAddress peerAddr_;

    ConnectionCallback connectionCallback_;  // 新连接回调
    MessageCallback messageCallback_; // 读写消息回调
    WriteCompleteCallback writeCompleteCallback_; // 消息发送完成以后的回调
    HighWaterMarkCallback highWaterMarkCallback_;
    CloseCallback closeCallback_;

    size_t highWaterMark_;
    Buffer inputBuffer_; // 接收数据的缓冲区
    Buffer outputBuffer_; // 发送数据缓冲区
};
