#include <iostream>
#include "EventLoop.h"
#include "InetAddress.h"
#include "TcpServer.h"
#include "TcpConnection.h"
#include "Logger.h"
#include "Buffer.h"
#define PORT 2024
#define MAX_EVENTS_NUMBER 5
using namespace std;



class EchoServer
{
public:
    EchoServer(EventLoop *loop,
            const InetAddress &addr, 
            const std::string &name)
        : server_(loop, addr, name)
        , loop_(loop)
    {
        server_.setConnectionCallback(
            std::bind(&EchoServer::onConnection, this, std::placeholders::_1)
        );
            
        server_.setMessageCallback(
            std::bind(&EchoServer::onMessage, this,
                std::placeholders::_1, std::placeholders::_2)
        );

        
        server_.setThreadNum(3);
    }
    void start(){
        server_.start();
    }
private:
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
            LOG_INFO("Connection UP : %s", conn->peerAddress().toIpPort().c_str());
        else
            LOG_INFO("Connection DOWN : %s", conn->peerAddress().toIpPort().c_str());
    }

    void onMessage(const TcpConnectionPtr &conn,
                   Buffer *buf)
    {
        std::string msg = buf->retrieveAllString();
        conn->send(msg);	
        // conn->shutdown(); 
    }
    EventLoop *loop_;
    TcpServer server_;
};

int main(){
    EventLoop loop;

    InetAddress addr(PORT);
    
    EchoServer server(&loop, addr, "EchoServer-01");

    server.start(); 

    loop.loop();

    return 0;
}