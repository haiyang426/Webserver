#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include <functional>
#include <vector>
#include <memory>
#include <atomic>
#include <mutex>
#include "noncopyable.h"
#include "CurrentThread.h"

class Channel;
class EPollPoller;


class EventLoop : noncopyable
{
public:
    typedef std::function<void()> Functor;

    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    void runInLoop(Functor cb); 
    void queueInLoop(Functor cb); 
    void wakeup();

    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel *channel);

    bool isInLoopThread() const {return threadId_ == CurrentThread::tid();}

private:
    void handleRead(); 
    void doPendingFunctors();

    using ChannelList = std::vector<Channel*>;
    std::atomic<bool> looping_; 
    std::atomic<bool> quit_; 
    std::atomic<bool> callingPendingFunctors_; 
    const pid_t threadId_; 

    std::unique_ptr<EPollPoller> poller_; 

    int wakeupFd_; 
    std::unique_ptr<Channel> wakeupChannel_;
    ChannelList activeChannels_;
    Channel *currentActiveChannel_;
    std::vector<Functor> pendingFunctors_; 
    std::mutex mutex_; 
}
#endif