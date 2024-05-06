# WebServer
用C++实现的高性能WEB服务器，参考了muduo网络的实现方法


## Technical points
1. 使用Epoll边沿触发的IO多路复用技术，非阻塞IO，使用Reactor模式
2. 采用one EventLoop per thread实现主从reactor网络模型
3. 使用eventfd实现了线程的异步唤醒
4. 使用智能指针等RAII机制
5. 使用双缓冲区技术实现了简单的异步日志系统
6. 利用更多C++11的特性

## To be completed 待实现
1. 使用内存池减少内存分配和释放的开销，以及减少内存碎片
2. 优化代码
3. 支持HTTP2.0

## usage with echo server
    mkdir build
    cd build

    cmake ..
    make

    ##server
    ./bin/Webserver

    ##client
    nc 127.0.0.1 2024


## usage with http server
    mkdir build
    cd build

    cmake ..
    make

    ##server
    ./bin/http_server

    ##webbench
    ./webbench -c 1000 -t 10 http://127.0.0.1:2024/

## 测试环境
云服务器
Ubuntu:22.04
2核8G
![alt text](image-2.png)


## 博客记录
[IO复用](https://www.hystack.cn/webserver01/)
[Muduo三大核心类的学习](https://www.hystack.cn/wbserver02/)

## Thanks

[Muduo](https://github.com/chenshuo/muduo)  
[Webserver](https://github.com/linyacool/WebServer)  
[cpp-project-structure](https://github.com/hattonl/cpp-project-structure)