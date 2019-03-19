/*
* Simple stream-based logger for C++11.
*
* Adapted from
*   http://vilipetek.com/2014/04/17/thread-safe-simple-logger-in-c11/
*/

#ifndef WAPPERLOGGER_H
#define WAPPERLOGGER_H

#include <sstream>

namespace utils {
namespace wapper {
namespace log {

using namespace std::placeholders;

/**
 * @brief The Level enum 定义日志等级
 */
enum ELogLevel
{
    EN_TRACE,
    EN_DEBUG,
    EN_INFO,
    EN_WARNING,
    EN_ERROR,
    EN_FATAL,
    EN_OFF
};

/**
* A simple stream-based logger.
*/
class LoggerBase
{
    /**
     * @brief The Logstream class 拼接日志内容，并将拼接完成的日志内容输出到真正的日志对象中
     */
    class Logstream : public std::ostringstream
    {

        typedef std::function<ELogLevel()> LogLevel;
        typedef std::function<void(ELogLevel, std::string)> LogOutput;

    public:

        /**
         * @brief Logstream
         * @param logger
         * @param l
         */
        Logstream(LoggerBase &logger, ELogLevel loglevel) : m_logger(logger), m_level(loglevel){}

        /**
       * @brief Logstream
       * @param ls
       */
        Logstream(const Logstream &ls) : m_logger(ls.m_logger), m_level(ls.m_level) {}

        ~Logstream()
        {
            // 析构时将拼接完成的日志流输出给真正的日志对象
            if (m_logger.level() <= m_level)
            {
                m_logger.log(m_level, this->str());
            }
        }

    private:

        /**
         * @brief m_logger
         */
        LoggerBase &m_logger;

        /**
         * @brief m_level 当前输出日志的等级
         */
        ELogLevel m_level;
    };

public:

    /**
     * @brief Logger 构造函数
     * @param loglevel 默认日志等级
     */
    LoggerBase(ELogLevel loglevel = ELogLevel::EN_DEBUG) : m_log_level(loglevel) {}

    virtual ~LoggerBase(){}

    /**
     * @brief level 设置日志等级
     * @param lev
     */
    void level(ELogLevel lev) { m_log_level = lev; }

    /**
     * @brief level 获取当前日志等级
     * @return
     */
    ELogLevel level() { return m_log_level; }

    /**
     * @brief log 日志输出接口
     * @param lev 打印等级
     * @param message 日志内容
     */
    virtual void log(ELogLevel out_lev, const std::string &message) = 0;

    /**
     * @brief operator () 重载括号运算符, logger(lev) << message
     * @param lev   打印等级
     * @return  日志流对象
     */
    Logstream operator()(ELogLevel lev = ELogLevel::EN_INFO)
    {
        return Logstream(*this, lev);
    }

    /**
        日志打印方法
     * @brief trace     跟踪日志
     * @brief debug     调试日志
     * @brief info      普通日志
     * @brief warning   警告级别日志
     * @brief error     错误日志
     * @brief fatal     致命日志
     * @return          返回一个流对象可使用“<<”拼接日志，调用结束Logstream对象析构时将流输出到日志
     */
    Logstream trace() { return (*this)(ELogLevel::EN_TRACE); }
    Logstream debug() { return (*this)(ELogLevel::EN_DEBUG); }
    Logstream info() { return (*this)(ELogLevel::EN_INFO); }
    Logstream warning() { return (*this)(ELogLevel::EN_WARNING); }
    Logstream error() { return (*this)(ELogLevel::EN_ERROR); }
    Logstream fatal() { return (*this)(ELogLevel::EN_FATAL); }

private:

    /**
     * @brief m_log_level 设置默认日志等级，打印日志时低于该等级的日志不输出
     */
    ELogLevel m_log_level;
};
typedef std::shared_ptr<LoggerBase> LoggerBasePtr;

} // End log
} // End wapper
} // End utils

#endif // WAPPERLOGGER_H
