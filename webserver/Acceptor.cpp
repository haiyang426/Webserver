#include "Acceptor.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "InetAddress.h"

static int createNonblocking() 
{

    int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK| SOCK_CLOEXEC, IPPROTO_TCP);
    return sockfd;
    
}

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reuseport)
:
    loop_(loop),
    acceptSocket_(createNonblocking()),
    acceptChannel_(loop, acceptSocket_.fd()),
    listenning_(false)
{
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.setReusePort(true); 
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor()
{
    acceptChannel_.remove();
}

void Acceptor::listen()
{
    listenning_ = true;
    acceptSocket_.listening();
    acceptChannel_.enableReading();
}

void Acceptor::handleRead()
{
    InetAddress peerAddr;

    int connfd = acceptSocket_.accept(&peerAddr);
    
    if(connfd >= 0)
    {
        if(newConnectionCallback_)
            newConnectionCallback_(connfd, peerAddr);
        else
            close(connfd); 
    }
}