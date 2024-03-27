#include "Logger.h"
#include "HttpServer.h"
#include "HttpContext.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "TcpConnection.h"
// class TcpConnection;

using namespace std::placeholders;

void defaultHttpCallback(const HttpRequest&, HttpResponse* resp)
{
    resp->setStatusCode(HttpResponse::k404NotFound);
    resp->setStatusMessage("Not Found");
    resp->setCloseConnection(true);
}

HttpServer::HttpServer(EventLoop* loop,
                       const InetAddress& listenAddr,
                       const string& name)
  : server_(loop, listenAddr, name),
    httpCallback_(defaultHttpCallback),
    loop_(loop)
{
    server_.setConnectionCallback(
        std::bind(&HttpServer::onConnection, this, _1));
    server_.setMessageCallback(
        std::bind(&HttpServer::onMessage, this, _1, _2, _3));
}

void HttpServer::start()
{
    server_.start();
}

void HttpServer::onConnection(const TcpConnectionPtr& conn)
{
    if (conn->connected())
    {
        LOG_INFO("Connection UP : %s", conn->peerAddress().toIpPort().c_str());
        conn->setContext(HttpContext());
    }
    else
    {
        LOG_INFO("Connection DOWN : %s", conn->peerAddress().toIpPort().c_str());
    }
}

void HttpServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf, TimeStamp receiveTime)
{
    HttpContext* context = boost::any_cast<HttpContext>(conn->getMutableContext());

    
    if (!context->parseRequest(buf, receiveTime))
    {
        
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        conn->shutdown();
    }
    
    if (context->gotAll())
    {
        // LOG_INFO("Message UP 2 : %s", conn->peerAddress().toIpPort().c_str());
        onRequest(conn, context->request());
        context->reset();
    }
    // LOG_INFO("Message UP 3 : %s", conn->peerAddress().toIpPort().c_str());
}

void HttpServer::onRequest(const TcpConnectionPtr& conn, const HttpRequest& req)
{
    const string& connection = req.getHeader("Connection");
    // std::cout<<connection<<endl;
    bool close = connection == "close" ||
                (req.getVersion() == HttpRequest::kHttp10 && connection != "Keep-Alive");

    // std::cout<<"ssss"<<std::endl;
    HttpResponse response(close);
    httpCallback_(req, &response);

    Buffer *buf = new Buffer(2024);
    // 
    
    // std::string msg = "HTTP/1.1 400 Bad Request\r\n\r\n";
    // LOG_INFO("Message UP 2 : %s", msg);
    
    response.appendToBuffer(buf);
    // std::cout<<"ssss"<<std::endl;
    std::string msg = buf->retrieveAllString();
    // std::cout<<msg<<endl;
    conn->send(msg);
    // std::cout<<"ssss"<<std::endl;
    if (response.closeConnection())
    {
        conn->shutdown();
    }
    // std::cout<<"ssss"<<std::endl;
}