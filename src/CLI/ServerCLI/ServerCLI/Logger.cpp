#define _CRT_SECURE_NO_WARNINGS
// C++ Header File(s)
#include "pch.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// Code Specific Header Files(s)
#include "Logger.h"

using namespace std;
using namespace CPlusPlusLogging;

Logger* Logger::m_Instance = 0;

// Log file name. File name should be change from here only
const string logFileName = "MyLogFile.log";

using convert_t = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_t, wchar_t> str;

Logger::Logger()
{
	m_File.open(logFileName.c_str(), ios::out | ios::app);
	m_LogLevel = LOG_LEVEL_TRACE;
	m_LogType = CONSOLE;

	// Initialize mutex
#ifdef WIN32
	InitializeCriticalSection(&m_Mutex);
#else
	int ret = 0;
	ret = pthread_mutexattr_settype(&m_Attr, PTHREAD_MUTEX_ERRORCHECK_NP);
	if (ret != 0)
	{
		printf("Logger::Logger() -- Mutex attribute not initialize!!\n");
		exit(0);
	}
	ret = pthread_mutex_init(&m_Mutex, &m_Attr);
	if (ret != 0)
	{
		printf("Logger::Logger() -- Mutex not initialize!!\n");
		exit(0);
	}
#endif
}

Logger::~Logger()
{
	m_File.close();
#ifdef WIN32
	DeleteCriticalSection(&m_Mutex);
#else
	pthread_mutexattr_destroy(&m_Attr);
	pthread_mutex_destroy(&m_Mutex);
#endif
}

Logger* Logger::getInstance() throw ()
{
	if (m_Instance == 0)
	{
		m_Instance = new Logger();
	}
	return m_Instance;
}

void Logger::lock()
{
#ifdef WIN32
	EnterCriticalSection(&m_Mutex);
#else
	pthread_mutex_lock(&m_Mutex);
#endif
}

void Logger::unlock()
{
#ifdef WIN32
	LeaveCriticalSection(&m_Mutex);
#else
	pthread_mutex_unlock(&m_Mutex);
#endif
}

void Logger::logIntoFile(std::string& data)
{
	lock();
	m_File << getCurrentTime() << "  " << data << endl;
	unlock();
}


void Logger::logOnConsole(std::string& data)
{
	cout << getCurrentTime() << "  " << data << endl;
}


string Logger::getCurrentTime()
{
	string currTime;
	//Current date/time based on current time
	time_t now = time(0);
	// Convert current time to string
	currTime.assign(ctime(&now));

	// Last charactor of currentTime is "\n", so remove it
	string currentTime = currTime.substr(0, currTime.size() - 1);
	return currentTime;
}





// Interface for Info Log
void Logger::info(const char* text) throw()
{
	string data;
	data.append("[INFO]: ");
	data.append(text);

	if ((m_LogType == FILE_LOG) && (m_LogLevel >= LOG_LEVEL_INFO))
	{
		logIntoFile(data);
	}
	else if ((m_LogType == CONSOLE) && (m_LogLevel >= LOG_LEVEL_INFO))
	{
		logOnConsole(data);
	}
	else if ((m_LogType == BOTH_LOG) && (m_LogLevel >= LOG_LEVEL_INFO))
	{
		logIntoFile(data);
		logOnConsole(data);
	}
}

void Logger::info(std::string& text) throw()
{
	info(text.data());
}

void Logger::info(std::ostringstream& stream) throw()
{
	string text = stream.str();
	info(text.data());
}





// Interface for Debug Log
void Logger::debug(const char* text) throw()
{
	string data;
	data.append("[DEBUG]: ");
	data.append(text);

	if ((m_LogType == FILE_LOG) && (m_LogLevel >= LOG_LEVEL_DEBUG))
	{
		logIntoFile(data);
	}
	else if ((m_LogType == CONSOLE) && (m_LogLevel >= LOG_LEVEL_DEBUG))
	{
		logOnConsole(data);
	}
	else if ((m_LogType == BOTH_LOG) && (m_LogLevel >= LOG_LEVEL_DEBUG))
	{
		logIntoFile(data);
		logOnConsole(data);
	}
}


void Logger::debug(std::string& text) throw()
{
	debug(text.data());
}

void Logger::debug(std::ostringstream& stream) throw()
{
	string text = stream.str();
	debug(text.data());
}





// Interface for Trace Log
void Logger::trace(const char* text) throw()
{
	string data;
	data.append("[TRACE]: ");
	data.append(text);

	if ((m_LogType == FILE_LOG) && (m_LogLevel >= LOG_LEVEL_TRACE))
	{
		logIntoFile(data);
	}
	else if ((m_LogType == CONSOLE) && (m_LogLevel >= LOG_LEVEL_TRACE))
	{
		logOnConsole(data);
	}
	else if ((m_LogType == BOTH_LOG) && (m_LogLevel >= LOG_LEVEL_TRACE))
	{
		logIntoFile(data);
		logOnConsole(data);
	}
}


void Logger::trace(std::string& text) throw()
{
	trace(text.data());
}

void Logger::trace(std::ostringstream& stream) throw()
{
	string text = stream.str();
	trace(text.data());
}





// Interface for Error Log
void Logger::error(const char* text) throw()
{
	string data;
	data.append("[ERROR]: ");
	data.append(text);

	// ERROR must be capture
	if (m_LogType == FILE_LOG)
	{
		logIntoFile(data);
	}
	else if (m_LogType == CONSOLE)
	{
		logOnConsole(data);
	}
	else if ((m_LogType == BOTH_LOG))
	{
		logIntoFile(data);
		logOnConsole(data);
	}
}


void Logger::error(std::string& text) throw()
{
	error(text.data());
}

void Logger::error(std::ostringstream& stream) throw()
{
	string text = stream.str();
	error(text.data());
}





// Interfaces to control log levels
void Logger::updateLogLevel(LogLevel logLevel)
{
	m_LogLevel = logLevel;
}

// Enable all log levels
void Logger::enaleLog()
{
	m_LogLevel = ENABLE_ALL_LOG;
}

// Disable all log levels
void Logger::disableLog()
{
	m_LogLevel = DISABLE_LOG;
}

// Interfaces to control log Types
void Logger::updateLogType(LogType logType)
{
	m_LogType = logType;
}

void Logger::enableConsoleLogging()
{
	m_LogType = CONSOLE;
}

void Logger::enableFileLogging()
{
	m_LogType = FILE_LOG;
}