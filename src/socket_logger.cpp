#include "socket_logger.hpp"
#include "utils.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>

SocketLogger::SocketLogger(const std::string& host, int port,MessageLevel level) 
    : host_(host), port_(port),default_level_(level),socketFd_(-1) {
    connect();
}

SocketLogger::~SocketLogger() {
    if (socketFd_ != -1) {
        close(socketFd_);
    }
}

void SocketLogger::connect() {
    socketFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd_ == -1) {
        throw std::runtime_error("Failed to create socket");
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);

    if (inet_pton(AF_INET, host_.c_str(), &server_addr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid address");
    }

    if (::connect(socketFd_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("Connection failed");
    }
}

void SocketLogger::log(const std::string& msg, MessageLevel level) {
     if(level<default_level_) {
        return ;
    }
    std::lock_guard<std::mutex> lock(socket_mutex_);
    if (socketFd_ == -1) {
        throw std::runtime_error("Socket is not connected");
    }

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_c), "%F %T") 
        << " [" << levelToString(level) << "] " 
        << msg << "\n";

    std::string log_msg = oss.str();
    if (send(socketFd_, log_msg.c_str(), log_msg.size(), 0) < 0) {
        throw std::runtime_error("Failed to send message");
    }
}
void SocketLogger::log(const std::string&msg){
    std::lock_guard<std::mutex> lock(socket_mutex_);
    log(msg,default_level_);
}
void SocketLogger::change_level(MessageLevel new_level){
    default_level_=new_level;
}