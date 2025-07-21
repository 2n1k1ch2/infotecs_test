#pragma once
#include "logger.hpp"
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <algorithm>
class LogQueue {
    std::queue<std::pair<std::string, MessageLevel>> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stop_ = false;

public:
    void push(const std::string& message, MessageLevel level);
    bool pop(std::pair<std::string, MessageLevel>& result);
    void stop();
};

void logWorker(Logger& logger, LogQueue& queue);

