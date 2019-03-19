# wapper_logger
包装日志库的中间件
一、什么是wapper_logger
  wapper_logger 是用来包装日志库的中间件，各插件持有日志装饰器基类指针进行日志打印，应用层实现自己的日志包装类，工程初始化过程构造并将实例对象传递给各插件。
  
二、Hello world

#include "logger.hpp"
#include <iostream>
#include <unistd.h>

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
    return 0;
}
