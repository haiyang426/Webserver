#include "HttpResponse.h"
#include "Buffer.h"
#include <iostream>
#include <sstream>
#include <string>
void HttpResponse::appendToBuffer(Buffer* output) const
{
    std::ostringstream oss;
    // snprintf(buf, sizeof(buf), "HTTP/1.1 %d ", statusCode_);
    oss<<"HTTP/1.1 "<<statusCode_<<" ";
    output->append(oss.str());
    output->append(statusMessage_);
    output->append("\r\n");
    oss.clear();

    if(closeConnection_)
    {
        output->append("Connection: close\r\n");
    }
    else
    {
        oss<<"Content-Length: "<<body_.length()<<"\r\n";
        output->append(oss.str());
        output->append("Connection: Keep-Alive\r\n");
    }
    for (const auto& header : headers_)
    {
        output->append(header.first);
        output->append(": ");
        output->append(header.second);
        output->append("\r\n");
    }
    output->append("\r\n");
    output->append(body_);
};