#pragma once
#include <memory>
#include <functional>
// #include "TcpConnection.h"
// #include "Buffer.h"
class Buffer;
class TcpConnection;

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using ConnectionCallback = std::function<void (const TcpConnectionPtr&)>;
using CloseCallback = std::function<void (const TcpConnectionPtr&)>;
using WriteCompleteCallback = std::function<void (const TcpConnectionPtr&)>;
using MessageCallback = std::function<void (const TcpConnectionPtr&, Buffer*)>;
using HighWaterMarkCallback = std::function<void (const TcpConnectionPtr&, size_t)>;