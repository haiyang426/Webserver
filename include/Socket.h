#pragma once
#include "noncopyable.h"

class InetAddress;

class Socket : public noncopyable
{
public:
    explicit Socket(int sockfd) : sockfd_(sockfd){}
    ~Socket();

public:
    int fd() const {return sockfd_;}
    void bindAddress(const InetAddress &localaddr); 
    void listening(); 
    int accept(InetAddress *peeradd); 
    void shutdownWrite();  


    void setTcpNoDelay(bool on); 
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);

private:
    const int sockfd_; 
};