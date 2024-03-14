#include "Poller.h"

Poller::Poller(EventLoop *loop) : ownerLoop_(loop) {}

Poller::~Poller() = default;

bool Poller::hasChannel(Channel *channel) const
{
    //map: (sockfd, channel*)
    auto it = channels_.find(channel->fd());
    return it != channels_.end() && it->second == channel;
}