#include "TcpServer.h"
#include "Logger.h"
#include <strings.h>
#include "TcpConnection.h"

using namespace std;

// static EventLoop* CheckLoopNotNull(EventLoop* loop)
// {
//     if(loop == nullptr)
//     {
//         LOG_FATAL("%s:%s:%d mainLoop is null \b", __FILE__, __FUNCTION__, __LINE__);
//     }
//     return loop;
// }

TcpServer::TcpServer(EventLoop* loop, 
                     const InetAddress &listenAddr, 
                     const string &nameArg, 
                     Option option)
:
    loop_(loop),
    ipPort_(listenAddr.toIpPort()),
    name_(nameArg),
    acceptor_(new Acceptor(loop, listenAddr , option == kReusePort)),
    threadPool_(new EventLoopThreadPool(loop, name_)),
    connectionCallback_(),
    messageCallback_(),
    started_(0),
    nextConnId_(1)
{
    acceptor_->setNewConnectionCallback(bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer()
{
    for(auto &item : connections_)
    {

        TcpConnectionPtr conn(item.second);
        item.second.reset(); 
        conn->getLoop()->runInLoop(bind(&TcpConnection::connectDestroyed, conn));
    }
}

void TcpServer::setThreadNum(int numThreads)
{
    threadPool_->setThreadNum(numThreads);
}

void TcpServer::start()
{
    if(started_++ == 0)
    {
        threadPool_->start(threadInitCallback_);
        loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
    }
}

void TcpServer::newConnection(int sockfd, const InetAddress &peerAddr)
{
    EventLoop *ioLoop = threadPool_->getNextLoop();
    char buf[64] = {0};
    snprintf(buf, sizeof(buf), "-%s#%d", ipPort_.c_str(), nextConnId_);
    ++nextConnId_; 
    string connName = name_ + buf;
    // LOG_INFO("TcpServer::newConnection [%s] - new connection [%s] from %s \n",
    //     name_.c_str(), connName.c_str(), peerAddr.toIpPort().c_str());

    sockaddr_in local;
    bzero(&local, sizeof(local));
    socklen_t addrlen = sizeof(local);
    if(getsockname(sockfd, (sockaddr*)&local, &addrlen) < 0)
    {
        LOG_ERROR("sockets::getLocalAddr");
    }

    InetAddress localAddr(local);

    TcpConnectionPtr conn(new TcpConnection(ioLoop, connName, sockfd, localAddr, peerAddr));
    connections_[connName] = conn;

    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(bind(&TcpServer::removeConnection, this, std::placeholders::_1));

    ioLoop->runInLoop(bind(&TcpConnection::connectEstablished, conn));
    
}

void TcpServer::removeConnection(const TcpConnectionPtr &conn)
{
    loop_->runInLoop(
        bind(&TcpServer::removeConnectionInLoop, this, conn)
    );
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr &conn)
{
    // LOG_INFO("TcpServer::removeConnectionInLoop [%s] - connection %s\n", 
    //     name_.c_str(), conn->name().c_str());
    connections_.erase(conn->name());
    EventLoop *ioLoop = conn->getLoop();
    ioLoop->queueInLoop(bind(&TcpConnection::connectDestroyed, conn));
}