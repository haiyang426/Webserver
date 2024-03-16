#include "Channel.h"
#include "EventLoop.h"
#include <sys/epoll.h>  
#include <memory>
using namespace std;
const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop* loop, int fd)
: 
	loop_(loop),
	fd_(fd),
	events_(0),
	revents_(0),
	index_(-1),
	tied_(false)

{
    
}

Channel::~Channel(){}

void Channel::tie(const shared_ptr<void>& obj)
{

	tie_ = obj;
	tied_ = true;
}

void Channel::HandlerEvent()
{
    if(tied_){
        shared_ptr<void> guard = tie_.lock();
        if (guard)
            HandleEventWithGuard();
    }
    else
        HandleEventWithGuard();
}

void Channel::HandleEventWithGuard()
{
    if((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN))
    {
        if(closeCallback_)
            closeCallback_();
    }
    if(revents_ & EPOLLERR)
    {
        if(errorCallback_)
            errorCallback_();
    }
    if(revents_ & (EPOLLIN | EPOLLPRI))
    {
        if(readCallback_)
            readCallback_();
    }
    if (revents_ & EPOLLOUT)
    {
        if (writeCallback_)
            writeCallback_();
    }
}


void Channel::update()
{
	addedToLoop_ = true;
	loop_->updateChannel(this);
}

void Channel::remove()
{	
	loop_->removeChannel(this);
}