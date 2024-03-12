#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include <functional>
#include <vector>
#include <memory>
#include <atomic>
#include "webserver/base/noncopyable.h"

class Channel;
class Poller;


class EventLoop : noncopyable
{
public:
    typedef std::function<void()> Functor;

    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel *channel);


private:
    typedef std::vector<Channel*> ChannelList;

    std::unique_ptr<Poller> poller_;

    ChannelList activeChannels_;
    Channel* currentActiveChannel_;

    void handleRead();

    std::atomic<bool> quit_;
    std::atomic<bool> looping_;
}
#endif