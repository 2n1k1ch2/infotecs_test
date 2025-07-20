#pragma once
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class SocketLogger {
public:
    SocketLogger(const std::string& ip, int port);
    ~SocketLogger();
    void log(const std::string* msg, MessageLevel level )
    void log(const std::string& msg);
    void connect()
    void change_level(MessageLevel level)
private:
    int socketFd;
    sockaddr_in serverAddr;
};