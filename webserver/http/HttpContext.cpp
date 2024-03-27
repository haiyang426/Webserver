#include "Buffer.h"
#include "HttpContext.h"
#include "Logger.h"
#include <iostream>

void printSubstring(const char* start, const char* end) {
    while (start != end) {
        std::cout << *start;
        ++start;
    }
    std::cout << std::endl;
}

bool HttpContext::processRequestLine(const char* begin, const char* end)
{
    bool succeed = false;
    const char* start = begin;
    const char* space = std::find(start, end, ' ');

    // printSubstring(begin, end);

    if(space!=end && request_.setMethod(start, space))
    {
        start = space+1;
        space = std::find(start, end, ' ');
        if (space != end){
            const char* question = std::find(start, space, '?');
            if(question != space)
            {
                request_.setPath(start, question);
                request_.setQuery(question, space);
            }
            else
            {
                request_.setPath(start, space);
            }
            start = space+1;
            // printSubstring(start, end);
            succeed = end -start == 8 && std::equal(start, end-1, "HTTP/1.");
            // 
            if (succeed)
            {
                if(*(end-1)=='1')
                {
                    request_.setVersion(HttpRequest::kHttp11);
                }
                else if(*(end - 1) == '0')
                {
                    request_.setVersion(HttpRequest::kHttp10);
                }
                else
                {
                    succeed=false;
                }
            }
        }
        
    }
    // std::cout<<succeed<<std::endl;
    return succeed;
}

bool HttpContext::parseRequest(Buffer* buf, TimeStamp receiveTime)
{
    bool ok = true;
    bool hasMore = true;
    while(hasMore)
    {
        if(state_ == kExpectRequestLine)
        {
            const char* crlf = buf->findCRLF();
            
            // cout<<buf->beginWrite() - buf->peek()<<endl;
            if(crlf)
            {
                // LOG_INFO("have");
                ok = processRequestLine(buf->peek(), crlf);
                // LOG_INFO("Message UP : %s", crlf-1);
                if (ok)
                {
                    request_.setReceiveTime(receiveTime);
                    buf->retrieveUntil(crlf + 2);
                    state_ = kExpectHeaders;
                }
                else
                {
                    hasMore = false;
                }
                
            }
            else
            {
                // LOG_INFO("not");
                hasMore = false;
            }
            // LOG_INFO("state : %d", state_);
        }
        else if(state_ == kExpectHeaders)
        {
            const char* crlf = buf->findCRLF();
            // std::cout<<
            
            if(crlf)
            {
                // 
                const char* colon = std::find(buf->peek(), crlf, ':');
                // printSubstring(buf->peek(), colon);
                if(colon != crlf)
                {
                    request_.addHeader(buf->peek(), colon, crlf);
                }
                else
                {
                    state_ = kGotAll;
                    hasMore = false;
                }

                buf->retrieveUntil(crlf+2);
            }
            else
            {
                hasMore = false;
            }
        }
        else if(state_ ==kExpectBody)
        {

        }
    }
    // std::cout<<"state_: "<<state_<<endl;
    return ok;
}