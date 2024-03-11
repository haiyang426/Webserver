#include "webserver/poller/EPollPoller.h"
#include "webserver/channel/Channel.h"

#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <sys/epoll.h>

#include <iostream>


EPollPoller::EPollPoller()
: epollfd_(epoll_create1(EPOLL_CLOEXEC)),
  events_(kInitEventListSize)
{
    if (epollfd_ < 0)
    {
        std::cout<<"sss"<<std::endl;
    }
}


EPollPoller::~EPollPoller()
{
  close(epollfd_);
}

void EPollPoller::poll(int timeoutMs, ChannelList* activeChannels)
{
    int numEvents = epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs);

    int savedErrno = errno;

    if (numEvents > 0)
    {
        fillActiveChannels(numEvents, activeChannels);
        if(numEvents == events_.size())
        {
            events_.resize(events_.size()*2);
        }
    }
    else if (numEvents > 0)
    {

    }
    else
    {

    }
}

void EPollPoller::fillActiveChannels(int numEvents, ChannelList* activeChannels) const
{
    
}