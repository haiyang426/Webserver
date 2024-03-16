#include "EventLoopThreadPool.h"
#include "EventLoopThread.h"
#include <memory>
using namespace std;

EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop, 
                                        const std::string &nameArg)
:   baseLoop_(baseLoop),
    name_(nameArg),
    started_(false),
    numThreads_(0),
    next_(0)
{
    
}

EventLoopThreadPool::~EventLoopThreadPool()
{

}

void EventLoopThreadPool::start(const ThreadInitCallback &cb)
{
    started_ = true;
    for(int i = 0; i < numThreads_; i++)
    {
        char buf[name_.size() + 32];
        snprintf(buf, sizeof(buf), "%s%d",name_.c_str(),i);

        EventLoopThread *t = new EventLoopThread(cb, buf);

        threads_.push_back(std::unique_ptr<EventLoopThread>(t));

        loops_.push_back(t->startLoop());
    }
    if(numThreads_ == 0)
    {
        cb(baseLoop_);
    }
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
    EventLoop *loop = baseLoop_;
    if(!loops_.empty())
    {
        loop = loops_[next_];
        ++next_;
        if(next_ >= loops_.size())
            next_ = 0;
    }
    return loop;
}

vector<EventLoop*> EventLoopThreadPool::getAllGroups()
{
    if(loops_.empty())
        return std::vector<EventLoop*> {baseLoop_};
    else
        return loops_;
}