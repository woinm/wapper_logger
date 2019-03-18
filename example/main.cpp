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

int main()
{
    cout << "Hello World!" << endl;

    //
    if (!load_logger_config("config/log4cxx.conf"))
    {
        std::cout << "load logger config failed!" << std::endl;
        return 0;
    }

    log4cxx::LoggerPtr log4 = log4cxx::Logger::getLogger("client");


    LoggerBasePtr logger = std::make_shared<CWapperLog4cxx>(ELogLevel::EN_INFO, log4);
    logger->trace()   << "trace ==> hello world";
    logger->debug()   << "debug ==> hello world";
    logger->info()    << "info  ==> hello world";
    logger->warning() << "warning ==> hello world";
    logger->error()   << "error ==> hello world";
    logger->fatal()   << "fatal ==> hello world";

    return 0;
}
