#include "HttpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"
#define PORT 2024

int main(){
    EventLoop loop;

    InetAddress addr(PORT);
    
    HttpServer server(&loop, addr, "EchoServer-01");

    server.setThreadNum(12);

    server.start(); 

    loop.loop();

    return 0;
}