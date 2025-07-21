#pragma once
#include <string>
#include <fstream>
#include <chrono>
#include <mutex>

enum class MessageLevel {
    DEBUG,
    INFO,
    ERROR,
    INVALID
};

class Logger {
public:
    Logger(const std::string& filename, MessageLevel minLevel = MessageLevel::INFO);
    ~Logger();

    void log(const std::string& message, MessageLevel level);
    void log(const std::string& message);
    void change_level(MessageLevel newLevel);

private:
    std::ofstream logFile_;
    MessageLevel default_level_;
    std::mutex logMutex_;

    
};