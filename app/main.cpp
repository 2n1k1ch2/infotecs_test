#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include "../inc/logger.hpp"

class LogQueue {
    std::queue<std::pair<std::string, MessageLevel>> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stop_ = false;

public:
    void push(const std::string& message, MessageLevel level) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.emplace(message, level);
        cv_.notify_one();
    }

    bool pop(std::pair<std::string, MessageLevel>& result) {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] { return !queue_.empty() || stop_; });
        
        if (stop_ && queue_.empty()) return false;
        
        result = queue_.front();
        queue_.pop();
        return true;
    }

    void stop() {
        std::lock_guard<std::mutex> lock(mutex_);
        stop_ = true;
        cv_.notify_all();
    }
};

void logWorker(Logger& logger, LogQueue& queue) {
    while (true) {
        std::pair<std::string, MessageLevel> task;
        if (!queue.pop(task)) break;
        
        if (task.second == MessageLevel::INVALID) {
            logger.log(task.first);
        } else {
            logger.log(task.first, task.second);
        }
    }
}

bool isValidLevel(const std::string& word) {
    static const std::vector<std::string> valid_levels = {"DEBUG", "INFO", "ERROR"};
    return std::find(valid_levels.begin(), valid_levels.end(), word) != valid_levels.end();
}

std::pair<std::string, MessageLevel> extractMessageAndLevel(const std::string& input) {
    size_t last_space = input.find_last_of(" \t");
    
    if (last_space == std::string::npos || input.empty()) {
        return {input, MessageLevel::INVALID};
    }
    
    std::string potential_level = input.substr(last_space + 1);
    std::string message = input.substr(0, last_space);
    
    if (isValidLevel(potential_level)) {
        MessageLevel level;
        if (potential_level == "DEBUG") level = MessageLevel::DEBUG;
        else if (potential_level == "INFO") level = MessageLevel::INFO;
        else level = MessageLevel::ERROR;
        return {message, level};
    }
    
    return {input, MessageLevel::INVALID};
}

int main( ) {

    Logger logger("log.txt", MessageLevel::INFO);
    LogQueue logQueue;
    
    // Запуск worker thread
    std::thread worker(logWorker, std::ref(logger), std::ref(logQueue));
    
    std::string msg;
    while (true) {
        std::cout << "> Введите сообщение: ";
        std::getline(std::cin, msg);
        
        if (msg == "exit") {
            logQueue.stop();
            break;
        }
        
        auto [message, level] = extractMessageAndLevel(msg);
        logQueue.push(message, level);
    }
    
    worker.join();
    return 0;
}