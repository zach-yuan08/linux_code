#ifndef __LOG_HPP__
#define __LOG_HPP__ // ？？？

#include <iostream>
#include <cstdio>
#include <string>
#include <filesystem> //C++17
#include <sstream>
#include <fstream>
#include <memory>
#include <ctime>
#include <unistd.h>
#include "Mutex.hpp"

namespace LogModule
{
    using namespace MutexModule;
    const std::string gsep = "\r\n";

    // 策略模式，基类
    class LogStrategy
    {
    public:
        ~LogStrategy() = default;                              // ？？？
        virtual void SyncLog(const std::string &message) = 0; // ？？？
    };

    // 向显示器打印
    class ConsoleLogStrategy : public LogStrategy
    {

    public:
        ConsoleLogStrategy()
        {
        }

        // 这里的message指的是日志内容（日志被分为属性和内容）
        void SyncLog(const std::string &message) override // ？？？
        {
            LockGuard lockguard(_mutex);
            std::cout << message << gsep;
        }

        ~ConsoleLogStrategy()
        {
        }

    private:
        Mutex _mutex;
    };

    // 向文件打印
    const std::string defaultpath = "./log";
    const std::string defaultfile = "my.log";
    class FileLogStrategy : public LogStrategy
    {
    public:
        FileLogStrategy(const std::string &path = defaultpath, const std::string &file = defaultfile)
            : _path(path),
              _file(file)
        {
            // 若不存在目录则创建
            LockGuard lockguard(_mutex);
            if (std::filesystem::exists(_path)) // ？？？
                return;
            try
            {
                std::filesystem::create_directories(_path);
            }
            catch (const std::filesystem::filesystem_error &e) // ???
            {
                std::cerr << e.what() << '\n'; // ???
            }
        }

        void SyncLog(const std::string &message) override
        {
            LockGuard lockfuard(_mutex);

            std::string filename = _path + (_path.back() == '/' ? "" : "/") + _file;
            std::ofstream out(filename, std::ios::app); // ???
            if (!out.is_open())
                return;
            out << message << gsep;
            out.close();
        }

        ~FileLogStrategy()
        {
        }

    private:
        std::string _path; // 文件路径
        std::string _file; // 文件名
        Mutex _mutex;
    };

    // 上面解决的问题是刷新策略(向哪里输出)，下面要解决“形成一条日志信息”
    // ???
    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    std::string Level2Str(LogLevel level)
    {
        // 将等级转化为字符串
        switch (level)
        {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
        }
    }

    std::string GetTimeStamp()
    {
        // 获取当前时间
        time_t curr = time(nullptr);
        struct tm curr_tm;            // ???
        localtime_r(&curr, &curr_tm); // ???
        char timebuffer[128];
        snprintf(timebuffer, sizeof(timebuffer), "%4d-%02d-%02d %02d:%02d:%02d",
                 curr_tm.tm_year + 1900,
                 curr_tm.tm_mon + 1,
                 curr_tm.tm_mday,
                 curr_tm.tm_hour,
                 curr_tm.tm_min,
                 curr_tm.tm_sec);
        return timebuffer;
    }

    // ************************************************ //
    // 上面解决刷新问题，下面解决生成日志信息
    class Logger
    {
    public:
        Logger()
        {
            EnableConsoleLogStrategy(); // ??? 
        }

        void EnableFileLogStrategy()
        {
            _fflush_strategy = std::make_unique<FileLogStrategy>(); // ???
        }
        void EnableConsoleLogStrategy()
        {
            _fflush_strategy = std::make_unique<ConsoleLogStrategy>();
        }

        // ********************************************************************* // 
        // 日志信息（内部类）
        class LogMessage
        {
        public:
            LogMessage(LogLevel &level, std::string &src_name, int line_number, Logger &logger)
                : _curr_time(GetTimeStamp()),
                  _level(level),
                  _pid(getpid()),
                  _src_name(src_name),
                  _line_number(line_number),
                  _logger(logger)

            {
                // 合并日志信息
                std::stringstream ss; // ??
                ss << "[" << _curr_time << "] "
                   << "[" << Level2Str(_level) << "] "
                   << "[" << _pid << "] "
                   << "[" << _src_name << "] "
                   << "[" << _line_number << "] "
                   << "- ";
                _loginfo = ss.str();
            }

            // 重载运算符 <<
            // ???
            template <typename T>
            LogMessage &operator<<(const T &info)
            {
                std::stringstream ss;
                ss << info;
                _loginfo += ss.str();
                return *this;
            }

            ~LogMessage()
            {
                // ???
                if (_logger._fflush_strategy)
                {
                    _logger._fflush_strategy->SyncLog(_loginfo);
                }
            }

        private:
            std::string _curr_time;
            LogLevel _level;
            pid_t _pid;
            std::string _src_name;
            int _line_number;
            std::string _loginfo; // 合并好的日志信息
            Logger &_logger;      // ???
        };

        // ********************************************************************* // 

        // ???
        LogMessage operator()(LogLevel level, std::string name, int line)
        {
            return LogMessage(level, name, line, *this);
        }

        ~Logger()
        {
        }

    private:
        std::unique_ptr<LogStrategy> _fflush_strategy; // ???
    };

    // 全局日志对象
    Logger logger; // 给宏使用的

    // 使用宏，简化用户操作，获取文件名和行号
    // ???
    #define LOG(level) logger(level, __FILE__, __LINE__)
    #define Enable_Console_Log_Strategy() logger.EnableConsoleLogStrategy()
    #define Enable_File_Log_Strategy() logger.EnableFileLogStrategy()
}

#endif