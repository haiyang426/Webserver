#pragma once
#include "noncopyable.h"
#include <functional>
#include <thread>
#include <memory>
#include <unistd.h>
#include <string>
#include <atomic>

class Thread : noncopyable
{
public:
    using ThreadFunc = std::function<void()>;
    explicit Thread(ThreadFunc, const std::string &name = std::string());
    ~Thread();
    void start();
    void join();

    bool started() const{return started_;}
    pid_t tid() const {return tid_;}
    static int numCreated(){return numCreated_;}
private:
    void setDefaultName();
    bool started_;
    bool joined_;
    pid_t tid_;

    ThreadFunc func_;
    std::string name_;
    std::shared_ptr<std::thread> thread_;
    static std::atomic<int> numCreated_;
};