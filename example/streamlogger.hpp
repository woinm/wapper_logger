#ifndef STREAMLOGGER_HPP
#define STREAMLOGGER_HPP

#include "logger.hpp"

namespace utils { namespace wrapper { namespace log {
/**
 * @brief The Wrapper2Cout class 日志输出到C++标准输出流
 */
class Wrapper2OoutStream : public LoggerBase
{
public:
    Wrapper2OoutStream(ELogLevel loglevel = ELogLevel::EN_DEBUG): LoggerBase(loglevel){}
    ~Wrapper2OoutStream(){}

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
typedef std::shared_ptr<Wrapper2OoutStream> Wrapper2CoutPtr;

}}}
#endif // STREAMLOGGER_HPP
