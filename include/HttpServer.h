#pragma once
#include <functional>
#include "TcpServer.h"
#include "noncopyable.h"
#include "EventLoop.h"
#include "InetAddress.h"

class HttpRequest;
class HttpResponse;
class TimeStamp;

class HttpServer : noncopyable
{
public:
    using HttpCallback = std::function<void(const HttpRequest&, HttpResponse*)>;

    HttpServer(EventLoop *loop,
            const InetAddress &addr, 
            const std::string &name);
        
    void setHttpCallback(const HttpCallback& cb)
    {
        httpCallback_ = cb;
    }

    void setThreadNum(int numThreads)
    {
        server_.setThreadNum(numThreads);
    }

    void start();

private:
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn,
                 Buffer* buf,
                 TimeStamp receiveTime);   

    void onRequest(const TcpConnectionPtr&, const HttpRequest&);
    
    EventLoop *loop_;
    TcpServer server_;
    HttpCallback httpCallback_;
};