#include "test_framework.hpp"
#include "logger.hpp"
#include "logqueue.hpp"
#include <string>
//Тест на проверку создания файла 
TEST(Logger_CreatesFile) {
    Logger logger("test.txt", MessageLevel::DEBUG);
    logger.log("Test message");
    std::ifstream file("test.txt");
    if (!file.is_open()) {
        throw std::runtime_error("File not created");
    }
}

TEST(Logger_WriteLogSuccess) {
    Logger logger("test.txt",MessageLevel::INFO);
    std::string SuccessWrite="Write in log" ;
    std::string NotSuccessWrite="Dont write in log";
    logger.log(SuccessWrite, MessageLevel::INFO);
    logger.log(NotSuccessWrite, MessageLevel::DEBUG);
    std::ifstream file("test.txt");
    std::string s;
    while(getline(file, s)) { 
        s+=s+'\n';
    }
    if(s.find(SuccessWrite)== std::string::npos && s.find(NotSuccessWrite) != std::string::npos){
        throw std::runtime_error("wrong level login");
    } 
}

TEST(LogQueue_BasicOperations) {
    LogQueue queue;
    queue.push("test1", MessageLevel::INFO);
    queue.push("test2", MessageLevel::DEBUG);
    
    std::pair<std::string, MessageLevel> item;
    if (!queue.pop(item) || item.first != "test1") {
        throw std::runtime_error("First item mismatch");
    }
    
    if (!queue.pop(item) || item.first != "test2") {
        throw std::runtime_error("Second item mismatch");
    }
    
    if (queue.pop(item)) {
        throw std::runtime_error("Queue should be empty");
    }
}
