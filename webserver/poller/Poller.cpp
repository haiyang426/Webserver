#include "webserver/poller/Poller.h"
#include "webserver/channel/Channel.h"

Poller::Poller(){}

Poller::~Poller() = default;

bool Poller::hasChannel(Channel* channel) const
{
  ChannelMap::const_iterator it = channels_.find(channel->fd());
  return it != channels_.end() && it->second == channel;
}