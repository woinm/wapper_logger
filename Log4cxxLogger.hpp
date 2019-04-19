#ifndef LOG4CXXLOGGER_H
#define LOG4CXXLOGGER_H

#include <log4cxx/log4cxx.h>
#include <log4cxx/logger.h>
#include <logger.hpp>

namespace utils { namespace wrapper { namespace log {

/** ==========回调函数========= */
/**
 * @brief IsEnabledCB log4cxx日志等级确认回调函数，判断是否开输出等级的日志
 */
typedef std::function<bool()> IsEnabledCB;

/**
 * @brief LogLevelCB 获取log4cxx等级的智能指针
 */
typedef std::function<log4cxx::LevelPtr()> LogLevelCB;

/**
 * @brief TForcedLogPtr log4cxx forcedlog函数指针类型，是重载函数，绑定函数指针时指定接口
 */
typedef void ((log4cxx::Logger::*TForcedLogPtr)(const log4cxx::LevelPtr&, const std::string&,const log4cxx::spi::LocationInfo&) const);

/**
 * @brief ForcedLogCB forcedlog回调函数类型
 */
typedef std::function<void(const log4cxx::LevelPtr&, const std::string&, const log4cxx::spi::LocationInfo&)> ForcedLogCB;

/**
 * @brief WrapperFunc forcedlog 包装函数类型
 */
typedef std::function<void (const std::string &)> LogWrapperCB;

#define LOG_LEVEL_CHECK(level) if(level < 0 || level >= ELogLevel::EN_OFF) return;

#define BIND_ENABLED(ptr) (IsEnabledCB)(std::bind(ptr, m_logger))
#define BIND_LEVEL(ptr) (LogLevelCB)(std::bind(ptr))
#define BIND_FORCEDLOG() (ForcedLogCB)(std::bind((TForcedLogPtr)(&log4cxx::Logger::forcedLog), m_logger, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))
#define BIND_WRAPPER(wrapperptr, enabledptr, leveptr) std::bind(wrapperptr, BIND_ENABLED(enabledptr), BIND_LEVEL(leveptr), BIND_FORCEDLOG(), std::placeholders::_1)

/**
 * @brief log_wrapper  log4cxx forcedlog回调函数
 * @param is_enabled  log4cxx::Logger::isxxxEnabled回调函数
 * @param level       回调函数，获取对应等级的日志指针
 * @param forcedlog   日志接口
 * @param message     日志内容
 */
void log_wrapper(IsEnabledCB is_enabled, LogLevelCB level, ForcedLogCB forcedlog, const std::string &message)
{
    if(is_enabled())
    {
        forcedlog(level(), message, LOG4CXX_LOCATION);
    }
}

/**
 * @brief The CLog4cxxLogger class log4cxx 日志包装类
 */
class CWrapperLog4cxx : public LoggerBase
{

public:
    /**
     * @brief CLog4cxxLogger
     * @param loglevel
     * @param log4
     */
    CWrapperLog4cxx(ELogLevel loglevel, log4cxx::LoggerPtr log4) : LoggerBase(loglevel), m_logger(log4)
    {
        // 根据日志等级绑定接口
        m_wrappers[ELogLevel::EN_TRACE]  = BIND_WRAPPER(&log_wrapper, &log4cxx::Logger::isTraceEnabled, &log4cxx::Level::getTrace);
        m_wrappers[ELogLevel::EN_DEBUG]  = BIND_WRAPPER(&log_wrapper, &log4cxx::Logger::isDebugEnabled, &log4cxx::Level::getDebug);
        m_wrappers[ELogLevel::EN_INFO]   = BIND_WRAPPER(&log_wrapper, &log4cxx::Logger::isInfoEnabled, &log4cxx::Level::getInfo);
        m_wrappers[ELogLevel::EN_WARNING]= BIND_WRAPPER(&log_wrapper, &log4cxx::Logger::isWarnEnabled, &log4cxx::Level::getWarn);
        m_wrappers[ELogLevel::EN_ERROR]  = BIND_WRAPPER(&log_wrapper, &log4cxx::Logger::isErrorEnabled, &log4cxx::Level::getError);
        m_wrappers[ELogLevel::EN_FATAL]  = BIND_WRAPPER(&log_wrapper, &log4cxx::Logger::isFatalEnabled, &log4cxx::Level::getFatal);
    }

    /**
     * @brief log 日志接口
     * @param lev 日志等级
     * @param message 日志内容
     */
    virtual void log(ELogLevel lev, const std::string &message)
    {
        LOG_LEVEL_CHECK(lev)
        if (m_logger)
        {
            m_wrappers[lev](message);
        }
    }
private:

    /**
     * @brief m_logger
     */
    log4cxx::LoggerPtr m_logger;
    LogWrapperCB m_wrappers[ELogLevel::EN_OFF];
};
typedef std::shared_ptr<CWrapperLog4cxx> CWrapperLog4cxxPtr;

}
}
}
#endif // LOG4CXXLOGGER_H
