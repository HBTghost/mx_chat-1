#pragma once
#ifndef _LOGGER_H_
#define _LOGGER_H_

// C++ Header File(s)
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <codecvt>
#include <locale>

#ifdef WIN32
// Win Socket Header File(s)
#include <Windows.h>
#include <process.h>
#else
// POSIX Socket Header File(s)
#include <errno.h>
#include <pthread.h>
#endif

namespace CPlusPlusLogging
{
    // Direct Interface for logging into log file or console using MACRO(s)
#define LOG_ERROR(x)    Logger::getInstance()->error(x)
#define LOG_INFO(x)     Logger::getInstance()->info(x)
#define LOG_TRACE(x)    Logger::getInstance()->trace(x)
#define LOG_DEBUG(x)    Logger::getInstance()->debug(x)

// enum for LOG_LEVEL
    typedef enum LOG_LEVEL
    {
        DISABLE_LOG = 1,
        LOG_LEVEL_INFO = 2,
        LOG_LEVEL_DEBUG = 3,
        LOG_LEVEL_TRACE = 4,
        LOG_LEVEL_ERROR = 5,
        ENABLE_ALL_LOG = 6,
    }LogLevel;

    // enum for LOG_TYPE
    typedef enum LOG_TYPE
    {
        NO_LOG = 1,
        CONSOLE = 2,
        FILE_LOG = 3,
        BOTH_LOG = 4
    }LogType;

    class Logger
    {
    public:
        static Logger* getInstance() throw ();

        // Interface for Info Log 
        void info(const char* text) throw();
        void info(std::string& text) throw();
        void info(std::ostringstream& stream) throw();

        // Interface for Debug log 
        void debug(const char* text) throw();
        void debug(std::string& text) throw();
        void debug(std::ostringstream& stream) throw();

        // Interface for Trace log 
        void trace(const char* text) throw();
        void trace(std::string& text) throw();
        void trace(std::ostringstream& stream) throw();

        // Interfaces to control log levels
        void updateLogLevel(LogLevel logLevel);
        void enaleLog();  // Enable all log levels
        void disableLog(); // Disable all log levels, except error

        // Interfaces to control log Types
        void updateLogType(LogType logType);
        void enableConsoleLogging();
        void enableFileLogging();

        // Interface for Error Log 
        void error(const char* text) throw();
        void error(std::string& text) throw();
        void error(std::ostringstream& stream) throw();

    protected:
        Logger();
        ~Logger();

        // Wrapper function for lock/unlock
        // For Extensible feature, lock and unlock should be in protected
        void lock();
        void unlock();

        std::string getCurrentTime();

    private:
        void logIntoFile(std::string& data);
        void logIntoFile(std::wstring& data);
        void logOnConsole(std::string& data);
        void logOnConsole(std::wstring& data);
        Logger(const Logger& obj) {}
        void operator=(const Logger& obj) {}

    private:
        static Logger* m_Instance;
        std::ofstream           m_File;

#ifdef	WIN32
        CRITICAL_SECTION        m_Mutex;
#else
        pthread_mutexattr_t     m_Attr;
        pthread_mutex_t         m_Mutex;
#endif

        LogLevel                m_LogLevel;
        LogType                 m_LogType;
    };

} // End of namespace

#endif // End of _LOGGER_H_