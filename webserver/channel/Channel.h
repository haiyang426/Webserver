#ifndef NET_CHANNEL_H
#define NET_CHANNEL_H
#include <functional>
#include <memory>
#include "webserver/base/noncopyable.h"

class EventLoop;

class Channel : noncopyable
{
public:
    Channel(EventLoop* loop, int fd);
    ~Channel();
    
    typedef std::function<void()> EventCallback;

    void HandlerEvent();
    void setReadCallback(EventCallback cb)
    { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb)
    { writeCallback_ = std::move(cb); }
    void setCloseCallback(EventCallback cb)
    { closeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb)
    { errorCallback_ = std::move(cb); }

    bool isNoneEvent() const { return events_ == kNoneEvent; }

    int fd() const { return fd_; }
    int events() const { return events_; }
    void set_revents(int revt) { revents_ = revt; }
    int index() { return index_; }
    void set_index(int idx) { index_ = idx; }

    EventLoop* ownerLoop() {return loop_;}

    void remove();

private:
    EventLoop* loop_;
    const int  fd_;
    int events_;
    int revents_;
    int index_;

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    void update();

    bool addedToLoop_;
    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;

    std::weak_ptr<void> tie_; 
    bool tied_;

    void HandleEventWithGuard();

};

#endif