#include "../inc/socket_logger.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>

SocketLogger::SocketLogger(const std::string& host, int port,MessageLevel level) 
    : host(host), port(port),default_level_(level),socket_fd(-1) {
    connect();
}

SocketLogger::~SocketLogger() {
    if (socket_fd != -1) {
        close(socket_fd);
    }
}

void SocketLogger::connect() {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        throw std::runtime_error("Failed to create socket");
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);

    if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid address");
    }

    if (::connect(socket_fd_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("Connection failed");
    }
}

void SocketLogger::log(const std::string& msg, MessageLevel level) {
     if(level<default_level_) {
        return 
    }
    std::lock_guard<std::mutex> lock(socket_mutex);
    if (socket_fd == -1) {
        throw std::runtime_error("Socket is not connected");
    }

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_c), "%F %T") 
        << " [" << MessageNames[static_cast<int>(level)] << "] " 
        << msg << "\n";

    std::string log_msg = oss.str();
    if (send(socket_fd, log_msg.c_str(), log_msg.size(), 0) < 0) {
        throw std::runtime_error("Failed to send message");
    }
}
void SocketLogger::log(const std::string&msg){
    std::lock_guard<std::mutex> lock(socket_mutex);
    log(msg,default_level);
}
void SocketLogger::change_default_level(MessageLevel new_level){
    default_level_=new_level;
}