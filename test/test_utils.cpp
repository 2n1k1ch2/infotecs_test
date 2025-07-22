#include "test_framework.hpp"
#include "utils.hpp"
#include "iostream"
TEST(isValidLevel_Test) {
    if (!(isValidLevel("DEBUG"))){
        throw std::runtime_error("Messeg Leveg 'DEBUG' has not been procced ");
    }
    if (!(isValidLevel("INFO"))){
        throw std::runtime_error("Messeg Leveg 'INFO' has not been procced   ");
    }
    if (!(isValidLevel("ERROR"))){
        throw std::runtime_error("Messeg Leveg 'ERROR' has not been procced   ");
    }
}
TEST(extractMessageAndLevel_Test) {

    std::pair<std::string ,MessageLevel> correct_result =
    {"Test case",MessageLevel::INFO};
    auto result = extractMessageAndLevel("Test case INFO");
    if(result!=correct_result){
        throw std::runtime_error("extractMessageAndLevel : 1 test is not working correct");
    }
    correct_result = {"INFO",MessageLevel::INVALID};
    result = extractMessageAndLevel("INFO");
    if(result!=correct_result){
        throw std::runtime_error("extractMessageAndLevel : 2 test is not working correct");
    }
    correct_result = {"",MessageLevel::INVALID};
    result = extractMessageAndLevel("");
     if(result!=correct_result){
        throw std::runtime_error("extractMessageAndLevel : 3 test is not working correct");
    }
}

TEST(parseLevel_test){
    if (auto res=parseLevel("DEBUG");res!=MessageLevel::DEBUG){
        throw std::runtime_error("DEBUG parse in not correct");
    }
    if (auto res=parseLevel("INFO");res!=MessageLevel::INFO){
        throw std::runtime_error("INFO parse in not correct");
    }
    if (auto res=parseLevel("ERROR");res!=MessageLevel::ERROR){
        throw std::runtime_error("ERROR parse in not correct");
    }

}
TEST(levelToString_test){
    if (auto res=levelToString(MessageLevel::DEBUG);res!="DEBUG"){
        throw std::runtime_error("DEBUG parse in not correct");
    }
    if (auto res=levelToString(MessageLevel::INFO);res!="INFO"){
        throw std::runtime_error("INFO parse in not correct");
    }
    if (auto res=levelToString(MessageLevel::ERROR);res!="ERROR"){
        throw std::runtime_error("ERROR parse in not correct");
    }
}
