#ifndef NET_POLLER_H
#define NET_POLLER_H
#include<vector>
#include<map>
#include "webserver/base/Timestamp.h"
#include "webserver/base/noncopyable.h"

class Channel;

class Poller : noncopyable
{
public:
    typedef std::vector<Channel*> ChannelList;
    Poller(){}
    ~Poller();

    virtual void poll(int timeoutMs, ChannelList* activeChannels) = 0;

    virtual void updateChannel(Channel* channel) = 0;

    virtual void removeChannel(Channel* channel) = 0;

    virtual bool hasChannel(Channel* channel) const;

    static Poller* newDefaultPoller();

protected:
    typedef std::map<int, Channel*> ChannelMap;
    ChannelMap channels_;
};
#endif
