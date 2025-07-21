#include "test_framework.hpp"
#include "logger.hpp"
#include "logqueue.hpp"
#include <string>
//Тест на проверку создания файла 
TEST(Logger_CreatesFile) {
    const std::string filename = "test_creates_file.txt";
    std::remove(filename.c_str());
    
    Logger logger(filename, MessageLevel::DEBUG);
    logger.log("Test message");
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("File not created");
    }
    file.close();
    std::remove(filename.c_str());
}
//Тест на корректное логирование.
TEST(Logger_WriteLogSuccess) {
    const std::string filename = "test_write_log.txt";
    std::remove(filename.c_str());
    
    Logger logger(filename, MessageLevel::INFO);
    std::string SuccessWrite = "Write in log";
    std::string NotSuccessWrite = "Dont write in log";
    
    logger.log(SuccessWrite, MessageLevel::INFO);
    logger.log(NotSuccessWrite, MessageLevel::DEBUG);
    
    std::ifstream file(filename);
    std::string content((std::istreambuf_iterator<char>(file)), 
                       std::istreambuf_iterator<char>());
    
    if(content.find(SuccessWrite) == std::string::npos) {
        throw std::runtime_error("Correct message not found");
    }
    
    if(content.find(NotSuccessWrite) != std::string::npos) {
        throw std::runtime_error("Wrong level message found");
    }
    
    file.close();
    std::remove(filename.c_str());
}
//Тест на работоспособности очереди
// TEST(LogQueue_BasicOperations) {
//     std::cout<<"TEST: LogQueue_BasicOperations";
//     LogQueue queue;
//     queue.push("test1", MessageLevel::INFO);
//     queue.push("test2", MessageLevel::DEBUG);
    
//     std::pair<std::string, MessageLevel> item;
//     if (!queue.pop(item) || item.first != "test1") {
//         throw std::runtime_error("First item mismatch");
//     }
    
//     if (!queue.pop(item) || item.first != "test2") {
//         throw std::runtime_error("Second item mismatch");
//     }
    
//     if (queue.pop(item)) {
//         throw std::runtime_error("Queue should be empty");
//     }
// }
//Тест на конкурентность
TEST(Concurrency_Test) {
    const std::string logfile = "concurrency_test.log";
    const int MESSAGES_PER_LOGGER = 1000;

    std::remove(logfile.c_str());

    Logger logger1(logfile, MessageLevel::DEBUG);
    Logger logger2(logfile, MessageLevel::DEBUG);
    LogQueue logQueue1;
    LogQueue logQueue2;


    std::thread worker1(logWorker, std::ref(logger1), std::ref(logQueue1));
    std::thread worker2(logWorker, std::ref(logger2), std::ref(logQueue2));


    auto sender = [](LogQueue& queue, int id) {
        for (int i = 0; i < MESSAGES_PER_LOGGER; ++i) {
            queue.push("Thread " + std::to_string(id) + " message " + std::to_string(i), 
                      MessageLevel::INFO);
        }
    };

    std::thread sender1(sender, std::ref(logQueue1), 1);
    std::thread sender2(sender, std::ref(logQueue2), 2);

    sender1.join();
    sender2.join();

    logQueue1.stop();
    logQueue2.stop();
    worker1.join();
    worker2.join();

    std::ifstream file(logfile);
    std::string line;
    int count = 0;
    
    while (std::getline(file, line)) {
        count++;
    }

    if (count != MESSAGES_PER_LOGGER * 2) {
        throw std::runtime_error("Expected " + 
              std::to_string(MESSAGES_PER_LOGGER * 2) + 
              " messages, got " + std::to_string(count));
    }
}
