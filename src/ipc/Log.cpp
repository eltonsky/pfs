#include "Log.h"

void Log::init(string confFile)
{
    string initFileName = confFile;

	log4cpp::PropertyConfigurator::configure(initFileName);
}


Log::~Log()
{
    try{
        log4cpp::Category::getRoot().shutdown();
    } catch(exception& e) {
        cout<<"Failed to shut down log4cpp : " << e.what()<<endl;
        std::abort();
    }
}


void Log::write(string msg){
    log4cpp::Category& root = log4cpp::Category::getRoot();

    stringstream ss;
    ss<<"<"<<(long int)syscall(SYS_gettid)<<"> "<<msg;

    switch(defaultLogLevel){
        case DEBUG:
            root.debug(ss.str());
            break;
        case INFO:
            root.info(ss.str());
            break;
        case NOTICE:
            root.notice(ss.str());
            break;
        case WARN:
            root.warn(ss.str());
            break;
        case ERROR:
            root.error(ss.str());
            break;
        default:
            root.error(ss.str());
    }
}


void Log::write(LogLevel level, string msg){
    log4cpp::Category& root = log4cpp::Category::getRoot();

    stringstream ss;
    ss<<"<"<<(long int)syscall(SYS_gettid)<<"> "<<msg;

    switch(level){
        case DEBUG:
            root.debug(ss.str());
            break;
        case INFO:
            root.info(ss.str());
            break;
        case NOTICE:
            root.notice(ss.str());
            break;
        case WARN:
            root.warn(ss.str());
            break;
        case ERROR:
            root.error(ss.str());
            break;
        default:
            root.error(ss.str());
    }
}


void Log::write(const char* format, ...){
    log4cpp::Category& root = log4cpp::Category::getRoot();

    string str;
    va_list va;
    va_start(va,format);
    getString(format,va, str);
    va_end(va);

    stringstream ss;
    ss<<"<"<<(long int)syscall(SYS_gettid)<<"> "<<str;

    switch(defaultLogLevel){
        case DEBUG:
            root.debug(ss.str());
            break;
        case INFO:
            root.info(ss.str());
            break;
        case NOTICE:
            root.notice(ss.str());
            break;
        case WARN:
            root.warn(ss.str());
            break;
        case ERROR:
            root.error(ss.str());
            break;
        default:
            root.error(ss.str());
    }
}


void Log::write(LogLevel level, const char* format, ...){
    log4cpp::Category& root = log4cpp::Category::getRoot();

    string str;
    va_list va;
    va_start(va,format);
    getString(format,va, str);
    va_end(va);

    stringstream ss;
    ss<<"<"<<(long int)syscall(SYS_gettid)<<"> "<<str;

    switch(level){
        case DEBUG:
            root.debug(ss.str());
            break;
        case INFO:
            root.info(ss.str());
            break;
        case NOTICE:
            root.notice(ss.str());
            break;
        case WARN:
            root.warn(ss.str());
            break;
        case ERROR:
            root.error(ss.str());
            break;
        default:
            root.error(ss.str());
    }
}


inline void Log::getString(const char* format, va_list va, string& str) {
    size_t size = 1024;
    char* buffer = new char[size];

    int n = vsnprintf(buffer, size, format, va);

    if ((n > -1) && (static_cast<size_t>(n) < size)) {
        str.assign(buffer);
        delete [] buffer;
    }
}


void Log::simple(){
    log4cpp::Appender *appender1 = new log4cpp::OstreamAppender("console", &std::cout);
	appender1->setLayout(new log4cpp::BasicLayout());

	log4cpp::Appender *appender2 = new log4cpp::FileAppender("default", "program.log");
	appender2->setLayout(new log4cpp::BasicLayout());

	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.setPriority(log4cpp::Priority::WARN);
	root.addAppender(appender1);

	log4cpp::Category& sub1 = log4cpp::Category::getInstance(std::string("sub1"));
	sub1.addAppender(appender2);

	// use of functions for logging messages
	root.error("root error");
	root.info("root info");
	sub1.error("sub1 error");
	sub1.warn("sub1 warn");

	// printf-style for logging variables
	root.warn("%d + %d == %s ?", 1, 1, "two");

	// use of streams for logging messages
	root << log4cpp::Priority::ERROR << "Streamed root error";
	root << log4cpp::Priority::INFO << "Streamed root info";
	sub1 << log4cpp::Priority::ERROR << "Streamed sub1 error";
	sub1 << log4cpp::Priority::WARN << "Streamed sub1 warn";

	// or this way:
	root.errorStream() << "Another streamed error";
}


void Log::Properties(){
    std::string initFileName = "/home/eltonsky/cpp/pfs/conf/log4cpp.properties";
	log4cpp::PropertyConfigurator::configure(initFileName);

	log4cpp::Category& root = log4cpp::Category::getRoot();

	log4cpp::Category& sub1 =
		log4cpp::Category::getInstance(std::string("sub1"));

	log4cpp::Category& sub2 =
		log4cpp::Category::getInstance(std::string("sub1.sub2"));

	root.warn("Storm is coming");

	sub1.debug("Received storm warning");
	sub1.info("Closing all hatches");

	sub2.debug("Hiding solar panels");
	sub2.error("Solar panels are blocked");
	sub2.debug("Applying protective shield");
	sub2.warn("Unfolding protective shield");
	sub2.info("Solar panels are shielded");

	sub1.info("All hatches closed");

	root.info("Ready for storm.");

	log4cpp::Category::shutdown();
}



