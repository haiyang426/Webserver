#include "Timestamp.h"
#include <iostream>
#include <ctime>

Timestamp Timestamp::now()
{
    return Timestamp(time(NULL));
}