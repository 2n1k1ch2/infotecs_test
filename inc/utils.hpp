#pragma once
#include <logger.hpp>
#include <string>
#include <algorithm>
#include <iostream>

bool isValidLevel(const std::string& word);
std::pair<std::string, MessageLevel> extractMessageAndLevel(const std::string& input);
MessageLevel parseLevel(const std::string& levelStr);