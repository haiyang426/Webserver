#ifndef BASE_TIMESTAMP_H
#define BASE_TIMESTAMP_H

#include <iostream>
#include <string>
class Timestamp
{

public:
    // TimeStamp();
    Timestamp() : microSecondsSinceEpoch_(0) {}
    explicit Timestamp(int64_t microSecondsSinceEpochArg): microSecondsSinceEpoch_(microSecondsSinceEpochArg) {}
    static Timestamp now();
    std::string toString() const;
    
private:
    int64_t microSecondsSinceEpoch_; 
};
#endif