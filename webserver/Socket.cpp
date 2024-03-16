#include "InetAddress.h"
#include "Socket.h"

#include <unistd.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <strings.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
using namespace std;

Socket::~Socket()
{
    close(sockfd_);
}

void Socket::bindAddress(const InetAddress &localaddr)
{
    bind(sockfd_, (sockaddr*)localaddr.getSockAddr(), sizeof(sockaddr_in));
}

void Socket::listening()
{
    listen(sockfd_, 1024);
}

int Socket::accept(InetAddress *peeraddr)
{
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    bzero(&addr, sizeof(addr));
    int connfd = accept4(sockfd_, (sockaddr*)&addr, &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (connfd >= 0)
    {
        peeraddr->setSockAddr(addr);
    }
    return connfd;
}

void Socket::shutdownWrite()
{
    shutdown(sockfd_, SHUT_WR);
}

void Socket::setReuseAddr(bool on)
{
    int optval = on ? 1 : 0;
    setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}
void Socket::setReusePort(bool on)
{
    int optval = on ? 1 : 0;
    setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
}
void Socket::setKeepAlive(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval));
}
void Socket::setTcpNoDelay(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
}