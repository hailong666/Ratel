#pragma once

#include <vector>
#include <unordered_map>
#include "noncopyable.h"
#include "Timestamp.h"

class Channel;
class EventLoop;
// muduo库中多路事件分发器的核心IO复用模块
class Poller:noncopyable
{ 
public:
    using ChannelList = std::vector<Channel*>;
    Poller(EventLoop *loop);
    virtual ~Poller() = default;
    //给所有的IO复用保留统一接口
    virtual Timestamp poll(int timeoutMs, ChannelList *activeChannels) = 0;
    virtual void updateChannel(Channel* channel) = 0;
    virtual void removeChannel(Channel* channel) = 0;

    // 判断参数channel是否在当前Poller当中
    bool hasChannel(Channel* channel) const;

    // 可以通过该接口获取默认的IO复用的具体实现
    static Poller* newDefaultPoller(EventLoop *loop);
protected:
    //map的key 是sockfd  vlaue是sockfd所属的channel通道
    using ChannelMap = std::unordered_map<int, Channel*>;
    ChannelMap channels_;
private:
    EventLoop *ownerLoop_; //Poller所属的事件循环EventLoop
};
