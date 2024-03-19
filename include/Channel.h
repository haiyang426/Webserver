#pragma once
#include <functional>
#include <memory>
#include "noncopyable.h"
#include "Timestamp.h"

class EventLoop;


class Channel : noncopyable
{
public:
    Channel(EventLoop* loop, int fd);
    ~Channel();
    
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(TimeStamp)>;

    void HandlerEvent(TimeStamp receive_time);
    
    void setReadCallback(ReadEventCallback cb)
    { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb)
    { writeCallback_ = std::move(cb); }
    void setCloseCallback(EventCallback cb)
    { closeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb)
    { errorCallback_ = std::move(cb); }

    void enableReading() {events_ |= kReadEvent; update();} 
    void disableReading() {events_ &= ~kReadEvent; update();}
    void enableWriting() {events_ |= kWriteEvent; update();}
    void disableWriting() {events_ &= ~kWriteEvent; update();}
    void disableAll() {events_ = kNoneEvent; update();}

    bool isWriting() const {return events_ & kWriteEvent;}
    bool isReading() const {return events_ & kReadEvent;} 
    bool isNoneEvent() const {return events_ == kNoneEvent;}

    int fd() const { return fd_; }
    int events() const { return events_; }
    void set_revents(int revt) { revents_ = revt; }
    int index() { return index_; }
    void set_index(int idx) { index_ = idx; }

    EventLoop* ownerLoop() {return loop_;}

    void remove();

    void tie(const std::shared_ptr<void>&);

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
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;

    std::weak_ptr<void> tie_; 
    bool tied_;

    void HandleEventWithGuard(TimeStamp receiveTime);;

};
