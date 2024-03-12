#include <signal.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include "webserver/event/EventLoop.h"

const int kPollTimeMs = 10000;

EventLoop::EventLoop()
:
poller_(new EPollPoller()),
currentActiveChannel_(nullptr),

{

}

EventLoop::~EventLoop()
{

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
    }
}
