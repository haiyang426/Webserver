#include "webserver/channel/Channel.h"
#include <sys/epoll.h>  
Channel::Channel(int fd)
 :fd_(fd),
  events_(0),
  revents_(0)
{
}

void Channel::handleEvent()
{
    if(revents_ & EPOLLIN)
    {
        readCallback_();
    }
}