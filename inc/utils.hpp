#pragma once
#include <logger.hpp>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
using std::string_literals::operator""s;
bool isValidLevel(const std::string& word);
std::pair<std::string, MessageLevel> extractMessageAndLevel(const std::string& input);
MessageLevel parseLevel(const std::string& levelStr);
std::string levelToString(MessageLevel level);