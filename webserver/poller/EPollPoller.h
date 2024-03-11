#ifndef POLLER_EPOLLPOLLER_H
#define POLLER_EPOLLPOLLER_H

#include <vector>
#include "webserver/poller/Poller.h"

class EPollPoller : public Poller
{
public:
    EPollPoller();
    ~EPollPoller() override;

    void poll(int timeoutMs, ChannelList* activeChannels) override;

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
}


#endif // POLLER_EPOLLPOLLER_H