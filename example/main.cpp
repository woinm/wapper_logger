#include "Log4cxxLogger.hpp"

#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/filewatchdog.h>

#include <iostream>
#include <unistd.h>

using namespace std;
using namespace utils::wapper::log;

/**
 * @brief load_logger_config 加载日志文件配置信息
 * @param file_path 配置文件路径
 * @return true:加载成功，false: 加载失败
 */
bool load_logger_config(const std::string &file_path)
{
    if (access(file_path.c_str(), F_OK | R_OK) != 0) {
        return false;
    }

    log4cxx::File config_file(file_path);
    log4cxx::PropertyConfigurator::configure(config_file);

    return true;
}

/**
 * @brief The Wapper2Cout class 日志输出到C++标准输出流
 */
class Wapper2OoutStream : public LoggerBase
{
public:
    Wapper2OoutStream(ELogLevel loglevel = ELogLevel::EN_DEBUG): LoggerBase(loglevel){}
    ~Wapper2OoutStream(){}

public:
    /**
     * @brief log
     * @param out_lev
     * @param message
     */
    void log(ELogLevel out_lev, const std::string &message)
    {
        const static char *LEVEL_STR[] =
        {
            "[Trace]    ", "[Debug] ", "[Info]  ", "[Warning]", "[Error]    ", "[Fatal] "
        };

        std::cout << LEVEL_STR[out_lev] << "\t" << message << std::endl;
    }
};
typedef std::shared_ptr<Wapper2OoutStream> Wapper2CoutPtr;

int main()
{
    std::cout << "========= Out to std cout <=========" << std::endl;
    LoggerBasePtr base_loger = std::make_shared<Wapper2OoutStream>(ELogLevel::EN_INFO);
    base_loger->trace()  << "Hello World!";
    base_loger->debug()  << "Hello World!";
    base_loger->info()   << "Hello World!";
    base_loger->warning()<< "Hello World!";
    base_loger->error()  << "Hello World!";

    cout << "\n" << std::endl;
    cout << "===========> Log4cxx loger <===========" << endl;
    if (!load_logger_config("config/log4cxx.conf"))
    {
        std::cout << "load logger config failed!" << std::endl;
        return 0;
    }

    log4cxx::LoggerPtr log4 = log4cxx::Logger::getLogger("client");
    LoggerBasePtr log4cxx_logger = std::make_shared<CWapperLog4cxx>(ELogLevel::EN_INFO, log4);
    log4cxx_logger->trace()   << "hello world!";
    log4cxx_logger->debug()   << "hello world!";
    log4cxx_logger->info()    << "hello world!";
    log4cxx_logger->warning() << "hello world!";
    log4cxx_logger->error()   << "hello world!";
    log4cxx_logger->fatal()   << "hello world!";

    return 0;
}
