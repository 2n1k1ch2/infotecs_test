#include <logqueue.hpp>

void LogQueue::push(const std::string& message, MessageLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.emplace(message, level);
    cv_.notify_one();
}

bool LogQueue::pop(std::pair<std::string, MessageLevel>& result) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return !queue_.empty() || stop_; });
        
    if (stop_ && queue_.empty()) return false;
        
    result = queue_.front();
    queue_.pop();
    return true;
}

void LogQueue::stop() {
    std::lock_guard<std::mutex> lock(mutex_);
    stop_ = true;
    cv_.notify_all();
}
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
