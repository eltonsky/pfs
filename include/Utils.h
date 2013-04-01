#ifndef UTILS_H
#define UTILS_H

#include <sys/time.h>

class Utils
{
    public:
        Utils();
        virtual ~Utils();

        static const long getTime();
    protected:
    private:
};

#endif // UTILS_H
