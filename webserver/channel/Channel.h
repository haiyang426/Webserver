#ifndef NET_CHANNEL_H
#define NET_CHANNEL_H
#include <functional>
#include <memory>

class Channel
{
public:
private:
    const int  fd_;
    int events_;
};

#endif