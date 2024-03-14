#include <signal.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <memory>
#include <mutex>
#include "webserver/event/EventLoop.h"
#include "CurrentThread.h"

const int kPollTimeMs = 10000;
__thread EventLoop* t_loopInThisThread = nullptr;

int createEventfd()
{
  int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  return evtfd;
}


EventLoop::EventLoop() : 
    looping_(false), 
    quit_(false), 
    callingPendingFunctors_(false), 
    threadId_(CurrentThread::tid()), 
    poller_(Poller::newDefaultPoller(this)), 
    wakeupFd_(createEventfd()), 
    wakeupChannel_(new Channel(this, wakeupFd_)),
    currentActiveChannel_(nullptr) 
{
    if(t_loopInThisThread)
    {

    }
    else
    {
        t_loopInThisThread = this;
    }

    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    close(wakeupFd_);
    t_loopInThisThread = nullptr;
}

void EventLoop::loop()
{
    looping_ = true;
    quit_ = false;
    while(!quit_)
    {
        activeChannels_.clear();
        poller_->poll(kPollTimeMs, &activeChannels_);

        for(Channel *channel : activeChannels_)
        {
            channel->handleEvent();
        }

        doPendingFunctors();
    }
}

void EventLoop::quit()
{
    quit_ = true;
    if(!isInLoopThread()) wakeup();
}

void EventLoop::runInLoop(Functor cb)
{
    if(isInLoopThread()){
        cb();
    }
    else{
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(Functor cb)
{
    {
        unique_lock<mutex> lock(mutex_);
        pendingFunctors_.emplace_back(cb);
    }


    if(!isInLoopThread() || callingPendingFunctors_) wakeup();
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = write(wakeupFd_, &one, sizeof(one));
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = read(wakeupFd_, &one, sizeof(one));
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;

   {
       unique_lock<mutex> lock(mutex_);
       functors.swap(pendingFunctors_);
   }

   for(const Functor &functor:functors)
   {
       functor();
   }
   callingPendingFunctors_ = false;

}


void EventLoop::updateChannel(Channel *channel)
{
    poller_->updateChannel(channel); //
}
void EventLoop::removeChannel(Channel *channel)
{
    poller_->removeChannel(channel);
}
bool EventLoop::hasChannel(Channel *channel)
{
    poller_->hasChannel(channel);
}