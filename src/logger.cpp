#include "../inc/logger.hpp"
#include <iostream>
#include <ctime>

Logger::Logger(const std::string& filename, MessageLevel minLevel)
    : logFile_(filename, std::ios::app), default_level_(minLevel) {
    if (!logFile_.is_open()) {
        std::cerr << "Error: Failed to open log file " << filename << std::endl;
    }
}

Logger::~Logger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

void Logger::log(const std::string& msg, MessageLevel level) {
    if (level < default_level_) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(logMutex_);
    
    if (logFile_.is_open()) {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        logFile_<< "[" << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << "] "
                << "[" << levelToString(level) << "] "
                << msg << std::endl;
    }
}

void Logger::log(const std::string& msg) {
    log(msg, default_level_);
}

void Logger::change_level(MessageLevel newLevel) {
    std::lock_guard<std::mutex> lock(logMutex_);
    default_level_ = newLevel;
}

std::string Logger::levelToString(MessageLevel level) {
    switch (level) {
        case MessageLevel::DEBUG: return "DEBUG";
        case MessageLevel::INFO:  return "INFO";
        case MessageLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}