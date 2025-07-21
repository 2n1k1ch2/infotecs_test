#include <iostream>
#include "logger.hpp"
#include "logqueue.hpp"
#include "utils.hpp"


int main(int argc, char* argv[]) {
    
    if (argc != 3) {
        std::cerr << "Неправильное кол-во аргументов , должно быть 2 ";
        return 1;
    }
    const std::string logfile = argv[1];
    const MessageLevel default_level = parseLevel(argv[2]);

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