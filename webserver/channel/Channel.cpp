#include "webserver/channel/Channel.h"
#include <sys/epoll.h>  

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

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