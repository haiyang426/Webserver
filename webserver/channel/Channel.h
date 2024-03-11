#ifndef NET_CHANNEL_H
#define NET_CHANNEL_H
#include <functional>
#include <memory>
#include "webserver/base/Timestamp.h"
#include "webserver/base/noncopyable.h"

class Channel : noncopyable
{
public:
    Channel(int fd);
    ~Channel(){}
    
    typedef std::function<void()> EventCallback;
    typedef std::function<void()> ReadEventCallback;

    void handleEvent();
    void setReadCallback(ReadEventCallback cb)
    { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb)
    { writeCallback_ = std::move(cb); }
    void setCloseCallback(EventCallback cb)
    { closeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb)
    { errorCallback_ = std::move(cb); }


    int fd() const { return fd_; }
    int events() const { return events_; }
    void set_revents(int revt) { revents_ = revt; }


private:
    const int  fd_;
    int events_;
    int revents_;


    void update();

    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;
};

#endif