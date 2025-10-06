#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

enum LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3
};

class Logger {
private:
    static Logger* instance;
    std::ofstream logFile;
    LogLevel currentLevel;
    
    Logger();
    ~Logger();
    
    // Delete copy constructor and assignment operator
    Logger(const Logger&);
    Logger& operator=(const Logger&);
    
    std::string getCurrentTime();
    std::string levelToString(LogLevel level);

public:
    static Logger& getInstance();
    static void destroy();
    
    void setLogLevel(LogLevel level);
    void setLogFile(const std::string& filename);
    
    void log(LogLevel level, const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
};

// Convenience macros
#define LOG_DEBUG(msg) Logger::getInstance().debug(msg)
#define LOG_INFO(msg) Logger::getInstance().info(msg)
#define LOG_WARNING(msg) Logger::getInstance().warning(msg)
#define LOG_ERROR(msg) Logger::getInstance().error(msg)
