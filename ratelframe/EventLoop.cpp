#include <sys/eventfd.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <memory>
#include "Channel.h"
#include "Poller.h"
#include "Logger.h"
#include "EventLoop.h"
// 防止一个线程创建多个EventLoop
__thread EventLoop *t_loopInThisThread = nullptr;

// 定义默认的Poller IO复用超时时间
const int kPollTimeMs = 10000;
// 创建wakeupfd, 用来唤醒subReactor处理新来的channel
int createEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0)
    {
        LOG_FATAL("eventfd error:%d", errno)
    }
    return evtfd;
}

EventLoop::EventLoop(/* args */)
    :looping_(false), 
    quit_(false), 
    callingPendingFunctors_(false),
    threadId_(CurrentThread::tid()), 
    poller_(Poller::newDefaultPoller(this)),
    wakeupFd_(createEventfd()),
    wakeupChannel_(new Channel(this, wakeupFd_)),
    currentActiveChannel_(nullptr)
{
    LOG_DEBUG("EventLoop created %p in thread %d \n", this, threadId_);
    if(t_loopInThisThread)
    {
        LOG_FATAL("Another EventLoop %p exists in this thread %d \n", t_loopInThisThread, threadId_);
    }
    else
    {
        t_loopInThisThread = this;
    }
    // 设置wakeupfd的事件类型以及发生事件后的
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    // 每一个eventloop 都将监听wakeupchannel的EPOLLIN读事件了
    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    ::close(wakeupFd_);
    t_loopInThisThread = nullptr;
}

void EventLoop::handleRead()
{
    uint16_t one = 1;
    ssize_t n = read(wakeupFd_, &one, sizeof one);
    if (n != sizeof one)
    {
        LOG_ERROR("EventLoop::handleRead() reads %lu bytes instead of 8", n);
    }
}
// 开启事件循环
void EventLoop::loop()
{
    looping_ = true;
    quit_ = false;

    LOG_INFO("EventLoop %p start looping \n", this);
    while (!quit_)
    {
        activateChannels_.clear();
        // 监听两类fd 一种是client的fd,一种wakeupfd 
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activateChannels_);
        for (Channel *channel : activateChannels_)
        {
            // Poller监听哪些channel发生事件了，然后上报给EventLoop, 通知channel处理相应的事件
            channel->handleEvent(pollReturnTime_);
        }
        // 执行当前EventLoop事件循环处理回调操作
        // IO线程 mainLoop accept fd 《= channel subloop  
        // mainLoop 事先注册一个回调cd (需要subloop来执行) wakeup subloop后，执行下面的方法，执行之前的mainloop注册的cb
        doPendingFunctors();
    }
    
    LOG_INFO("EventLoop %p stop looping. \n", this)
    looping_ = false;

}

// 退出事件循环
void EventLoop::quit()
{
    quit_ = true;

    if (!isInLoopThread()) // 如果是在其他线程中，调用的quit 在一个subloop中，调用了mainLoop的quit
    {
        wakeup();
    }
}
void EventLoop::updateChannel(Channel *channel)
{
    poller_->updateChannel(channel);
}
void EventLoop::removeChannel(Channel *channel)
{
    poller_->removeChannel(channel);
}
bool EventLoop::hasChannel(Channel *channel)
{

    return poller_->hasChannel(channel);
}
// 在当前loop中执行
void EventLoop::runInLoop(Functor cb)
{
    if (isInLoopThread()) // 在当前loop线程中执行cb
    {
        cb();
    }
    else // 非当前线程中执行cb，需要唤醒loop所在线程执行cb
    {
        queueInLoop(cb);
    }
}
// 把cb放入队列中，唤醒loop所在的线程，执行cb
void EventLoop::queueInLoop(Functor cb)
{
    {
        std::unique_lock<std::mutex> lock(mutex_);
        pendingFunctors_.emplace_back(cb);

    }
    // 唤醒对应执行回调的线程
    if(!isInLoopThread() || callingPendingFunctors_)  //当前loop正在执行回调，但是loop又有了新的回调，需要再唤醒
    {
        wakeup();
    }
}

// 唤醒loop所在的线程, 向wakeupfd里面写入数据，wakeupfdChannel发生读事件，当前loop线程会被唤醒
void EventLoop::wakeup()
{
    uint16_t one = 1;
    ssize_t n = write(wakeupFd_, &one, sizeof one);
    if (n != sizeof one)
    {
        LOG_ERROR("EventLoop::wakeup() writes %lu bytes install of 8 \n", n);
    }
}
// 执行回调   妙用
void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for (const Functor &functor : functors)
    {
        functor(); // 执行当前loop需要执行的回调操作
    }
    callingPendingFunctors_ = false;    

}