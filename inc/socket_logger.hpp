#pragma once
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "logger.hpp"
#include "mutex"
class SocketLogger {
public:
    SocketLogger(const std::string& host, int port, MessageLevel level);
    ~SocketLogger();
    void log(const std::string& msg, MessageLevel level );
    void log(const std::string& msg);
    void connect();
    void change_level(MessageLevel level);
private:
    int socketFd_;
    sockaddr_in serverAddr_;
    MessageLevel default_level_;
    std::mutex socket_mutex_;
    std::string host_;
    int port_;
};