#include "Utils.h"
#include "unistd.h"

Utils::Utils()
{
    //ctor
}


const long Utils::getTime() {
    timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}


Utils::~Utils()
{
    //dtor
}
