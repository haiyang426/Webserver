#pragma once

#include <iostream>
#include <string>
class TimeStamp
{
public:
    TimeStamp()
        : microSecondSinceEpoch_(0)
    {
    }
    explicit TimeStamp(int64_t microSecondSinceEpoch); 
    static TimeStamp now(); 
    std::string toString() const;
    
private:
    int64_t microSecondSinceEpoch_; 
};