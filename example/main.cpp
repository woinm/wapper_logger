#include <iostream>
#include <unistd.h>

#include "streamlogger.hpp"
#include "Log4cxxLogger.hpp"

#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/filewatchdog.h>

using namespace std;
using namespace utils::wrapper::log;

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

class Demo
{
public:
    Demo(){}
    Demo(LoggerBasePtr logger) : m_logger(logger){}
    ~Demo(){}
    void show()
    {
        m_logger->trace()  << "Hello World!";
        m_logger->debug()  << "Hello World!";
        m_logger->info()   << "Hello World!";
        m_logger->warning()<< "Hello World!";
        m_logger->error()  << "Hello World!";
    }
private:
    LoggerBasePtr m_logger;
};

int main()
{
    // 标准输出流日志
    LoggerBasePtr stream_logger = std::make_shared<Wrapper2OoutStream>(ELogLevel::EN_INFO);

    // log4cxx 日志
    if (!load_logger_config("config/log4cxx.conf"))
    {
        std::cout << "load logger config failed!" << std::endl;
        return 0;
    }
    log4cxx::LoggerPtr log4 = log4cxx::Logger::getLogger("client");
    LoggerBasePtr log4cxx_logger = std::make_shared<CWrapperLog4cxx>(ELogLevel::EN_INFO, log4);

    //
    Demo obj(stream_logger);
    obj.show();

    return 0;
}
