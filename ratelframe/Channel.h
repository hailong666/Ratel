#pragma once

#include <functional>
#include <memory>
#include "noncopyable.h"
#include "Timestamp.h"
// Channel 为通道， 封装了sockfd和感兴趣事件和处理回调
class EventLoop;
class Timestamp; 
class Channel : noncopyable
{
public:
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(Timestamp)>;

    Channel(EventLoop *loop, int fd);
    ~Channel();

    // fd得到poller通知以后，处理事件的。调用相应的回调
    void handleEvent(Timestamp receiverTime);

    // 设置回调函数对象
    void setReadCallback(ReadEventCallback cb){readCallback_ = std::move(cb);}
    void setWriteCallback(EventCallback cb){writeCallback_ = std::move(cb);}
    void setCloseCallback(EventCallback cb){closeCallback_ = std::move(cb);}
    void setErrorCallback(EventCallback cb){errorCallback_ = std::move(cb);}

    // 防止当channel被手动remove掉，channel还在执行回调操作
    void tie(const std::shared_ptr<void>&);

    int fd() const {return fd_;}

    int events() const {return events_;}
    void set_revents(int revt){ revents_ = revt;}
    // bool isNoneEvent() const {return events_ == kNoneEvent;}

    //设置fd相应的事件状态
    void enableReading() { events_ |= kReadEvent; update();}
    void disableReading() { events_ &= ~kReadEvent; update();}
    void enableWriting() { events_ |= kWriteEvent; update();}
    void disableWriting() { events_ &= ~kWriteEvent; update();}
    void disableAll() { events_ = kNoneEvent; update();}

    // 返回fd当前的事件状态
    bool isNoneEvent() const { return events_ == kNoneEvent;}
    bool isWriting() const { return events_ & kWriteEvent;}
    bool isReading() const { return events_ & kReadEvent;}

    int index() { return index_; }
    void set_index(int idx) { index_ = idx;}

    // one loop per thread
    EventLoop* ownerLoop() {return loop_;}
    void remove();

private:
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;
    void update();
    void handleEventWithGuard(Timestamp receiveTime);
    EventLoop *loop_; //事件循环
    const int fd_;    //fd
    int events_; //注册fd感兴趣事件
    int revents_; //poller返回的具体发生的事件
    int index_; //在poller中的下标

    std::weak_ptr<void> tie_; //指向自己shared_from_this
    bool tied_;

    // channel通道里面能够 获知fd最终发生具体事件revents，所以它负责调用具体事件的回调操作
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;

};

