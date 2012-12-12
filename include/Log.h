#ifndef LOG_H
#define LOG_H
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include "Config.h"
#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
#include <log4cpp/PropertyConfigurator.hh>
using namespace std;

enum LogLevel {DEBUG, WARN, INFO, ERROR};


class Log
{
    public:
        Log();
        virtual ~Log();

        static void init(string conf);
        static void write(string msg);
        static void write(LogLevel level, string msg);

        // examples.
        static void simple();

        static void Properties();
        // end of exmpales.

    protected:
        static const LogLevel defaultLogLevel = INFO;

    private:
};

#endif // LOG_H
