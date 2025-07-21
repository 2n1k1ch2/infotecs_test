#include "utils.hpp"
#include "logger.hpp"



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
MessageLevel parseLevel(const std::string& levelStr) {
    if (levelStr == "DEBUG") return MessageLevel::DEBUG;
    if (levelStr == "INFO") return MessageLevel::INFO;
    if (levelStr == "ERROR") return MessageLevel::ERROR;
    std::cerr << "Unknown level. Defaulting to INFO\n";
    return MessageLevel::INFO;
}
std::string levelToString(MessageLevel level) {
    switch (level) {
        case MessageLevel::DEBUG: return "DEBUG";
        case MessageLevel::INFO:  return "INFO";
        case MessageLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}