#ifndef NET_CHANNEL_H
#define NET_CHANNEL_H
#include <functional>
#include <memory>
#include "Timestamp.h"

class Channel
{
public:
    Channel(int fd);
    ~Channel();
    
    typedef std::function<void()> EventCallback;
    typedef std::function<void(Timestamp)> ReadEventCallback;

    void handleEvent(Timestamp receiveTime);
    void setReadCallback(ReadEventCallback cb)
    { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb)
    { writeCallback_ = std::move(cb); }
    void setCloseCallback(EventCallback cb)
    { closeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb)
    { errorCallback_ = std::move(cb); }

private:
    const int  fd_;
    int events_;
    int revents_;

    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;
};

#endif