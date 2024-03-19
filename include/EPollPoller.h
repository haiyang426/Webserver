#pragma once
#include <vector>
#include "Poller.h"
#include <sys/epoll.h>


class EPollPoller : public Poller
{
public:
    EPollPoller(EventLoop* loop);
    ~EPollPoller() override;

    TimeStamp poll(int timeoutMs, ChannelList *activeChannels) override;

    void updateChannel(Channel* channel) override;

    void removeChannel(Channel* channel) override;

private:
    static const int kInitEventListSize = 16;

    void fillActiveChannels(int numEvents,
                          ChannelList* activeChannels) const;

    void update(int operation, Channel* channel);

    typedef std::vector<struct epoll_event> EventList;

    int epollfd_;

    EventList events_;
};
