cmake_minimum_required(VERSION 3.10)
project(LoggerLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
add_library(logger_static STATIC src/logger.cpp)
target_include_directories(logger_static PUBLIC inc)

add_library(logger_shared SHARED src/logger.cpp)
target_include_directories(logger_shared PUBLIC inc)
set_target_properties(logger_shared PROPERTIES OUTPUT_NAME "logger")




add_executable(loggerapp 
app/main.cpp
src/logqueue.cpp
src/utils.cpp
)
target_link_libraries(loggerapp PRIVATE logger_static)

target_include_directories(loggerapp PRIVATE inc)